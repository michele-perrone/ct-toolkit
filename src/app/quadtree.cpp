#include "quadtree.hpp"

Coordinates::Coordinates(short int _y, short int _x)
{
    y = _y;
    x = _x;
}
Coordinates::Coordinates(short _y)
{
    y = _y;
    x = _y;
}
Coordinates::Coordinates()
{
    y = 0;
    x = 0;
}
Coordinates Coordinates::operator + (Coordinates &_coordinates)
{
    y += _coordinates.y;
    x += _coordinates.x;

    return *this;
}

Boundary::Boundary(Coordinates _top_left, Coordinates _bot_right)
{
    top_left = _top_left;
    bot_right = _bot_right;
}
Boundary::Boundary()
{
    top_left = Coordinates();
    bot_right = Coordinates();
}
Boundary Boundary::Nort_West()
{
    return Boundary(this->top_left,
                    Coordinates((short)floor((this->top_left.y + this->bot_right.y)/2.0),
                                (short)floor((this->top_left.x + this->bot_right.x)/2.0)));
}
Boundary Boundary::North_East()
{
    return Boundary(Coordinates(this->top_left.y, (short)ceil((this->top_left.x + this->bot_right.x)/2.0)),
                    Coordinates((short)floor((this->top_left.y + this->bot_right.y)/2.0), this->bot_right.x));
}
Boundary Boundary::South_East()
{
    return Boundary(Coordinates((short)ceil((this->top_left.y + this->bot_right.y)/2.0),
                                (short)ceil((this->top_left.x + this->bot_right.x)/2.0)),
                    this->bot_right);
}
Boundary Boundary::South_West()
{
    return Boundary(Coordinates((short)ceil((this->top_left.y + this->bot_right.y)/2.0), this->top_left.x),
                    Coordinates(this->bot_right.y, (short)floor((this->top_left.x + this->bot_right.x)/2.0)));
}

Node::Node(bool _is_leaf, Boundary _boundary, uint16_t _luminance,
     Node* _north_west, Node* _north_east, Node* _south_east, Node* _south_west)
{
    is_leaf = _is_leaf;
    boundary = _boundary;
    luminance = _luminance;
    north_west = _north_west;
    north_east = _north_east;
    south_east = _south_east;
    south_west = _south_west;
}
Node::Node(Boundary _boundary, short int _luminance)
{
    is_leaf = true;
    boundary = _boundary;
    luminance = _luminance;
    north_west = NULL;
    north_east = NULL;
    south_east = NULL;
    south_west = NULL;
}
Node::Node()
{
    is_leaf = true;
    boundary = Boundary();
    luminance = 0;
    north_west = NULL;
    north_east = NULL;
    south_east = NULL;
    south_west = NULL;
}

QuadTree::QuadTree(uint16_t (*_IMG)[MAX_IMG_RES][MAX_IMG_RES], int _IMG_resolution, short int _compression_parameter)
{
    IMG = _IMG;
    IMG_resolution = _IMG_resolution;
    compression_parameter = _compression_parameter;
    // Create the root node and set its boundaries
    root_node = new Node();
    root_node->boundary = Boundary(Coordinates(0), Coordinates(IMG_resolution-1));
    root_node->luminance = (*IMG)[0][0];
}

QuadTree::~QuadTree()
{
    remove_nodes(root_node);
}

void QuadTree::construct_tree(Node* node)
{
    // Used to store the boundary of a new child.
    Boundary child_boundary;

    // For each row inside the boundary...
    for(short int row_ctr = node->boundary.top_left.y;
        row_ctr <= node->boundary.bot_right.y;
        row_ctr++)
    {
        // For each column inside the boundary...
        for(short int column_ctr = node->boundary.top_left.x;
            column_ctr <= node->boundary.bot_right.x;
            column_ctr++)
        {
            // After having subdivided the current region in four subregions,
            // there is no need to inspect further.
            if(node->is_leaf == false)
            {
                return;
            }

            // If the lumimance of the top-left pixel is different from the current pixel...
            if(abs((node->luminance) - ((*IMG)[row_ctr][column_ctr])) > compression_parameter) /*  && node->is_leaf */
            {
                // ... then the node isn't a leaf.
                node->is_leaf = false;
                binarized_tree.push_back(1);

                // Four children will be created:
                // 1) Nort-West
                child_boundary = node->boundary.Nort_West();
                node->north_west = new Node(child_boundary, (*IMG)[child_boundary.top_left.y][child_boundary.top_left.x]);

                // 2) North-East
                child_boundary = node->boundary.North_East();
                node->north_east = new Node(child_boundary, (*IMG)[child_boundary.top_left.y][child_boundary.top_left.x]);


                // 3) South-East
                child_boundary = node->boundary.South_East();
                node->south_east = new Node(child_boundary, (*IMG)[child_boundary.top_left.y][child_boundary.top_left.x]);


                // 4) South-West
                child_boundary = node->boundary.South_West();
                node->south_west = new Node(child_boundary, (*IMG)[child_boundary.top_left.y][child_boundary.top_left.x]);

                // Proceed recursively.
                construct_tree(node->north_west);
                construct_tree(node->north_east);
                construct_tree(node->south_east);
                construct_tree(node->south_west);
            }
        }
    }

    if (node->is_leaf)
    {
        binarized_tree.push_back(0);
        BinaryTools::push_short_to_binary_vector(node->luminance, &binarized_tree);
    }

}

void QuadTree::reconstruct_IMG()
{
    unsigned long input_ctr = 0;
    reconstruct_recursively(root_node->boundary, &input_ctr);
}

void QuadTree::reconstruct_recursively(Boundary boundary, unsigned long* input_ctr)
{
    // If we have finished reading the bits
    if ((*input_ctr) >= binarized_tree.size())
    {
        return;
    }
    // If the region has to be subdivided in four subregions
    if (binarized_tree[*input_ctr] == 1)
    {
        (*input_ctr)++;
        Boundary boundary_NW = boundary.Nort_West();
        Boundary boundary_NE = boundary.North_East();
        Boundary boundary_SE = boundary.South_East();
        Boundary boundary_SW = boundary.South_West();

        reconstruct_recursively(boundary_NW, input_ctr);
        reconstruct_recursively(boundary_NE, input_ctr);
        reconstruct_recursively(boundary_SE, input_ctr);
        reconstruct_recursively(boundary_SW, input_ctr);

        return;
    }
    // If we have reached a region with the same color
    else if (binarized_tree[*input_ctr] == 0)
    {
        (*input_ctr)++;
        short current_colour = BinaryTools::read_short_from_binary_vector(binarized_tree, *input_ctr);
        (*input_ctr) += N_OF_BITS_PIXEL;
        colorize_region(boundary, current_colour);

        return;
    }
}

void QuadTree::colorize_region(Boundary boundary, short value)
{
    for (int i = boundary.top_left.y; i <= boundary.bot_right.y; i++)
    {
        for (int j = boundary.top_left.x; j <= boundary.bot_right.x; j++)
        {
            (*IMG)[i][j] = value;
        }
    }
}

void QuadTree::print_IMG()
{
    for (int i = 0; i < MAX_IMG_RES; i++)
    {
        for (int j = 0; j < MAX_IMG_RES; j++)
        {
            printf("%d ", (*IMG)[i][j]);
        }
        printf("\n");
    }
    return;
}

void QuadTree::write_binarized_tree_to_file(char* file_name)
{
    FILE* fout = fopen(file_name, "wb");
    unsigned long bits_to_write = 8 + 8 + binarized_tree.size();
    unsigned long bytes_to_write = ceil(bits_to_write/8.0);
    uint8_t bit_overhead = (bytes_to_write * 8) - bits_to_write;
    uint8_t current_byte = 0;

    // Add a bit overhead to the vector, so its size is a multiple of 8
    for(int overhead_ctr = 0; overhead_ctr < bit_overhead; overhead_ctr++)
    {
        binarized_tree.push_back(0);
    }

    // First 8 bits of the file store the resolution of the image
    current_byte = 9;
    fwrite(&current_byte, sizeof(uint8_t), 1, fout);

    // Next 8 bits indicate the bit overhead at the end of the file
    fwrite(&bit_overhead, sizeof(uint8_t), 1, fout);

    // Then write out the binarized tree
    for (unsigned long byte_ctr = 0; byte_ctr < bytes_to_write; byte_ctr++)
    {
        current_byte = BinaryTools::read_byte_from_binary_vector(binarized_tree, (byte_ctr * 8));
        fwrite(&current_byte, sizeof(uint8_t), 1, fout);
    }

    fclose(fout);

    return;
}

void QuadTree::read_binarized_tree_from_file(char* file_name)
{
    FILE* fin = fopen(file_name, "rb");
    uint8_t IMG_resolution_power;
    uint8_t bit_overhead;
    uint8_t current_byte;

    // First 8 bits of the file store the resolution of the image
    fread(&IMG_resolution_power, sizeof(uint8_t), 1, fin);
    IMG_resolution = pow(2, IMG_resolution_power);

    // Next 8 bits indicate the bit overhead at the end of the file
    fread(&bit_overhead, sizeof(uint8_t), 1, fin);

    // Then read the binarized tree
    while(fread(&current_byte, sizeof(uint8_t), 1, fin) != 0)
    {
        BinaryTools::push_byte_to_binary_vector(current_byte, &binarized_tree);
    }

    // After the vector is loaded, remove the bit overhead
    for(int overhead_ctr = 0; overhead_ctr < bit_overhead; overhead_ctr++)
    {
        binarized_tree.pop_back();
    }

    fclose(fin);

    return;
}

void QuadTree::remove_nodes(Node* root_node)
{
    if(root_node == NULL)
    {
        return;
    }

    if(root_node->is_leaf)
    {
        delete(root_node);

        return;
    }

    if(!root_node->is_leaf)
    {
        remove_nodes(root_node->north_west);
        remove_nodes(root_node->north_east);
        remove_nodes(root_node->south_east);
        remove_nodes(root_node->south_west);
    }

    return;
}
