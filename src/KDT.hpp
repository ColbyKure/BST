//
// KDT.hpp
// CSE 100 Project 1
//
// Last modified by Sunny Sun and Colby Kure on 01/19/2019
//
//

#ifndef KDT_HPP
#define KDT_HPP
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <limits>
#include <vector>

#include "BST.hpp"

using namespace std;

#define DELTA 0.00005

/**
 * A simple 2D Point struct with (x, y) coordinates.
 */
class Point {
public:
    double x, y;

    /** Default constructor: initializes point at (0, 0) */
    Point() : x(0.0), y(0.0) {}

    /** Constructor that initializes point to given coordinates (x, y) */
    Point(double x, double y) : x(x), y(y) {}

    /** Equals operator */
    bool operator==(const Point &other) const {
        return abs(x - other.x) < DELTA && abs(y - other.y) < DELTA;
    }

    /** Not-equals operator */
    bool operator!=(const Point &other) const {
        return abs(x - other.x) > DELTA || abs(y - other.y) > DELTA;
    }

    /** Less than operator */
    bool operator<(const Point &other) const {
        cerr << "This operator should not be called, you will need "
             << "check the x and y components each time" << endl;
        return false;
    }

    /** Returns the square of the Euclidean distance between points p1 and p2 */
    static double squareDistance(const Point &p1, const Point &p2) {
        double xdistance = (p2.x-p1.x) * (p2.x-p1.x);
        double ydistance = (p2.y-p1.y) * (p2.y-p1.y);
        return xdistance + ydistance;
    }

};

/** 
 * Overload operator<< to print a Point object to an ostream. 
 */
std::ostream &operator<<(std::ostream &out, const Point &data) {
    out << "(" << data.x << ", " << data.y << ")";
    return out;
}

bool xLessThan(const Point &p1, const Point &p2) { 
    return p1.x < p2.x; 
}

bool yLessThan(const Point &p1, const Point &p2) { 
    return p1.y < p2.y; 
}

/**
 * A 2D Tree with operations to find nearest points in 2D space.
 */
class KDT : public BST<Point> {
public:
    /** 
     * Given a vector of Point references, insert a copy into the KDT.
     *
     * Note. Because of the nature of the KDTree, a simple < operator
     *       will not be sufficient. You will need to check the x and y
     *       coordinates individually.
     * Note. Since KDT inherits from BST, you still have access to the
     *       root, isize, and iheight instance variables and should modify
     *       them accordingly.
     * Note. We have also provided a private helper method for the build
     *       function called buildSubset defined below.
     *
     * Parameters:
     *     points - a vector of points to add into a new KDT
     *
     * Returns:
     *     the number of elements in the built KDT
     */
    virtual unsigned int build(vector<Point> points) {
        //empty vector
        if(points.size() == 0){
            return 0;   
        }
        else if(points.size() == 1){
            root = new BSTNode<Point>(points[0]);
            isize = 1;
            iheight = 1;
            return isize;
        }
        //sorting points on the x dimension
        sort(points.begin(), points.end(), xLessThan);
        //find left median and make root node
        int index = (points.size()-1)/2;
        root = new BSTNode<Point>(points[index]);//median is the root of our tree
        int height = 1;
        isize = 1;
        int dimension = 1; //y dimension

        //build left subtree
        root->left = buildSubset(points, 0, index, dimension, height+1);
        if (root->left != nullptr){
            root->left->parent = root;
        }

        //build right subtree
        root->right = buildSubset(points, index+1, points.size(), dimension, height+1);
        if (root->right != nullptr){
            root->right->parent = root;
        }

        return isize;
    }

    /** 
     * Find the nearest neighbor to a given point.
     *
     * Note. THIS IS DIFFERENT THAN HOW THE PREVIOUS FIND FUNCTION WORKS
     * Note. Since KDT inherits from BST, you still have access to the
     *       root, isize, and iheight member variables.
     * Note. The same dimension ordering you used to build the tree must be used
     *       to search through it.
     * Note. We have added a private helper method for your convenience called
     *       findNNHelper defined below.
     *
     * Paremeters:
     *     p - the query point for which to find its nearest neighbor
     *
     * Returns:
     *     an iterator pointing at the nearest neighbor, or pointing at nullptr
     *     if tree is empty
     */
    virtual iterator findNearestNeighbor(const Point &p) const {
        if (root == nullptr){
            return typename KDT::iterator(0);
        }
        if (root->right == nullptr && root->left == nullptr){
            return typename KDT::iterator(root);    
        }
        double smallestSqD = numeric_limits<double>::max();
        BSTNode<Point> * closestPoint = nullptr;
        unsigned int dimension = 0;

        findNNHelper(root, p, &smallestSqD, &closestPoint, dimension);
        return typename KDT::iterator(closestPoint);
    }

    /** 
     * For the kd-tree, the find method should not be used. Use the function
     * findNearestNeighbor instead.
     */
    virtual iterator find(const Point &p) const override {
        cerr << "The KD Tree find method should not be called" << endl;
        cerr << "Use findNearestNeighbor instead" << endl;
        return 0;
    }

    /** 
     * For the kd-tree, the insert method should not be used. The tree is
     * built all at once. There is no need to add individual points.
     */
    virtual bool insert(const Point &p) override {
        cerr << "The KD Tree insert method should not be called" << endl;
        return false;
    }

private:
    /* 
     * Recursively add a subset of the array to the KD tree.
     *
     * Parameters:
     *     items - the list of points that are used to build the kd tree
     *     start - the starting index for this subsection of the tree
     *     end - the non-inclusive ending index for this subtree
     *     dimension - 0 if sorting along the x-axis, and 1 if sorting
     *                 along the y-axis
     * PostConditions: 
     *     the parent node should have its left and right children
     *     set if there are any available points and recursive
     *     calls should be made for the left and right subtree
     *     (again, if there are any remaining points).
     *
     * NOTE: 
     *     We gave you two methods: xLessThan and yLessThan. You may
     *     find them useful for the sort function from #include <algorithm>.
     */
    BSTNode<Point> *buildSubset(vector<Point> &points, 
                                unsigned int start,
                                unsigned int end, 
                                unsigned int dimension,
                                unsigned int height) {
        int localDimension;
        //base case
        if (start == end){
             return nullptr;   
        }
        //update height
        if(height > iheight){
            iheight = height;   
        }
        if (start == end-1){ //if theres only one node left
            //set start as new node
            BSTNode<Point> *last = new BSTNode<Point>(points[start]);
            isize++;
            return last; //a point
        }

        //sort along dimension
        if(dimension == 0){
            sort(points.begin()+ start, points.begin() + end, xLessThan);
            localDimension = 1; //y dimension    
        }
        else if (dimension == 1){
            sort(points.begin() + start, points.begin() + end, yLessThan);
            localDimension = 0; //x dimension    
        }

        unsigned int index = 0;
        if(start == end) {
            index = start;
        }
        else {
            index = ((end - start - 1)/2) + start;
        }
        //create new node from left median
        BSTNode<Point> *curr;
        curr = new BSTNode<Point>(points[index]);//median  of our subtree
        isize++;
        if(start != index) {
            curr->left = buildSubset(points, start, index, localDimension, height+1);
        }
        if (curr->left != nullptr){
            curr->left->parent = curr;
        }
        if(index+1 != end) {
            curr->right = buildSubset(points, index+1, end, localDimension, height+1);
        }
        if (curr->right != nullptr){
            curr->right->parent =  curr;
        }
       
        return curr;
    }


    /* 
     * Find the node in the subtree that is closest to the given point p
     *  
     * Parameters: 
     *     node - the root of the subtree being searched
     *     queryPoint - the point whose nearest neighbor should be found
     *     smallestSquareDistance - the smallest distance value found in
     *                              the tree so far which can be updated
     *     closestPoint - the node corresponding to the current closest
     *                    node in the tree which can be udpated
     *     dimension - flags whether the node parameter splits remaining
     *                 nodes in its subtree by the x or y dimension
     *                 (has the value of either 0 or 1)
     *
     * PostCondition:
     *     closestPoint points to the nearest neighbor
     */
    void findNNHelper(BSTNode<Point> *node, 
                      const Point &queryPoint,
                      double *smallestSquareDistance,
                      BSTNode<Point> **closestPoint,
                      unsigned int dimension) const {
        //first find leaf node
        BSTNode<Point> *curr = findLeafNode(node, queryPoint, dimension);
        double tmpDist = Point::squareDistance(curr->data, queryPoint);
        if(tmpDist < *smallestSquareDistance) {
            *closestPoint = curr;
            *smallestSquareDistance = tmpDist;
            if(tmpDist == 0) {
                return;
            }
        }
        if(checkSubtree(curr, queryPoint, smallestSquareDistance, 
                        closestPoint, dimension)) {
            if(curr->parent->left == curr) {
                if(curr->parent->right != nullptr) {
                    findNNHelper(curr->parent->right, queryPoint, 
                                 smallestSquareDistance, closestPoint, 
                                 dimension);
                }
            }
            else {
                if(curr->parent->left != nullptr) {
                    findNNHelper(curr->parent->left, queryPoint, 
                                 smallestSquareDistance, closestPoint, 
                                 dimension);
                }
            }
        }
        /*int localDimension;
        if(dimension == 0) {
            localDimension = 1;
        }
        else {
            localDimension = 0;
        }*/
        while(curr != node) {
            curr = curr->parent;
            double tmpDist = Point::squareDistance(curr->data, queryPoint);
            if(tmpDist < *smallestSquareDistance) {
                *closestPoint = curr;
                *smallestSquareDistance = tmpDist;
            }
            if(checkSubtree(curr, queryPoint, smallestSquareDistance, 
                            closestPoint, dimension)) {
                if(curr->parent->left == curr) {
                    if(curr->parent->right != nullptr) {
                        findNNHelper(curr->parent->right, queryPoint, 
                                     smallestSquareDistance, closestPoint, 
                                     dimension);
                    }
                }
                else {
                    if(curr->parent->left != nullptr) {
                        findNNHelper(curr->parent->left, queryPoint, 
                                     smallestSquareDistance, closestPoint, 
                                     dimension);
                    }
                }
            }
        }
        return;
    }

    static BSTNode<Point> *findLeafNode(BSTNode<Point> *node, 
                                        const Point &queryPoint,
                                        unsigned int dimension) {
        BSTNode<Point> *leaf;
        if(dimension == 0) {
            if(xLessThan(queryPoint, node->data)) {
                if(node->left == nullptr){
                    return node;
                }
                else {
                    leaf = findLeafNode(node->left, queryPoint, 1);
                }
            }
            else { //go right
                if(node->right == nullptr){
                    return node;
                }
                else {
                    leaf =findLeafNode(node->right, queryPoint, 1);
                }
            }
        }
        else {
            if(yLessThan(queryPoint, node->data)) {
                if(node->left == nullptr){
                    return node;
                }
                else {
                    leaf = findLeafNode(node->left, queryPoint, 0);
                }
            }
            else { //go right
                if(node->right == nullptr){
                    return node;
                }
                else {
                    leaf = findLeafNode(node->right, queryPoint, 0);
                }
            }
        }

        return leaf;
    }
    
    static bool checkSubtree(BSTNode<Point> *node, const Point &queryPoint, 
                      double *smallestSquareDistance, 
                      BSTNode<Point> **closestPoint, unsigned int dimension) {
        if(node->parent == nullptr) {
            return false;
        }
        double distance = Point::squareDistance(queryPoint, node->parent->data);
        if(distance < *smallestSquareDistance) {
            *closestPoint = node->parent;
            *smallestSquareDistance = distance;
        }
        double boundaryDist;
        if(dimension == 0) {
            boundaryDist = (node->data.x - queryPoint.x);
            boundaryDist = boundaryDist * boundaryDist; 
        }
        else { //check y dim
            boundaryDist = abs(node->data.y - queryPoint.y);
            boundaryDist = boundaryDist * boundaryDist; 
        }
        if(boundaryDist < *smallestSquareDistance) {
            return true;
        }
        return false;
    }
};

#endif  // KDT_HPP
