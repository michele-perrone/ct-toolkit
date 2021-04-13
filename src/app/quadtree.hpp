#include <iostream>
#include <cmath>
#include <vector>
#include <bitset>
#include <stdio.h>
#include "binarytools.hpp"
#define MAX_IMG_RES 512
#define MAX_NAME_LENGTH 64
using namespace std;


struct Coordinates
{
    // Y will be used for the rows;
    // X will be used for the columns.
    short int y;
    short int x;

    Coordinates(short int _y, short int _x);
    Coordinates(short _y);
    Coordinates();
    Coordinates operator + (Coordinates &_coordinates);
};

struct Boundary
{
    Coordinates top_left;
    Coordinates bot_right;
    Boundary(Coordinates _top_left, Coordinates _bot_right);
    Boundary();

    // The following methods calculate the four sub-boundaries of the current boundary.
    Boundary Nort_West();
    Boundary North_East();
    Boundary South_East();
    Boundary South_West();
};

// A region is a portion of the original image.
// When a region contains only pixels with the same luminance, it becomes a leaf of the quadtree.
// Otherwise, it becomes a node.
// Every node that isn't a leaf has exactly four children; a leaf has no children.
struct Node
{
    bool is_leaf;
    Boundary boundary;
    uint16_t luminance;

    // The children are in clockwise order.
    Node* north_west;
    Node* north_east;
    Node* south_east;
    Node* south_west;

    Node(bool _is_leaf, Boundary _boundary, uint16_t _luminance,
         Node* _north_west, Node* _north_east, Node* _south_east, Node* _south_west);
    Node(Boundary _boundary, uint16_t _luminance);
    Node();
};

struct QuadTree
{
    uint16_t (*IMG)[MAX_IMG_RES][MAX_IMG_RES];
    short int IMG_resolution;
    short int compression_parameter;
    Node* root_node;
    vector<bool> binarized_tree;

    QuadTree(uint16_t (*_IMG)[MAX_IMG_RES][MAX_IMG_RES], int _IMG_resolution, int16_t _compression_parameter);

    // Removes all dynamically allocated memory.
    ~QuadTree();

    // Subdivides the picture in four regions recursively.
    void construct_tree(Node* node);

    // Given a constructed tree, it reconstructs the original IMG.
    void reconstruct_IMG();

    // Prints the IMG as a matrix
    void print_IMG();

    void write_binarized_tree_to_file(const char *file_name);

    void read_binarized_tree_from_file(const char* file_name);

private:
    // Does the actual reconstruction job
    void reconstruct_recursively(Boundary boundary, unsigned long* input_ctr);

    // Assign a grayscale value to an entire region of an IMG.
    void colorize_region(Boundary boundary, short value);

    // Does the actual destructor job
    void remove_nodes(Node* root_node);


};
