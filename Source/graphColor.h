#ifndef _GRAPHCOLOR_H
#define _GRAPHCOLOR_H

#include "Module.decl.h" 

/*readonly*/
#define THRESHOLD (1)
extern CProxy_Main mainProxy;
extern AdjListType adjList_;
extern int vertices_;
extern int chromaticNum_;


class Main : public CBase_Main {
  private:
    std::string filename, newGraph;
  
  public:
    Main(CkArgMsg* msg);
    Main(CkMigrateMessage* msg);

    void parseCommandLine(int argc, char **argv);
    void readDataFromPython(int argc, char **argv);
    int getConservativeChromaticNum();
    
    void done();
    void populateInitialState(std::vector<vertex>&);
};

class Node: public CBase_Node {
  Node_SDAG_CODE

  private:
    std::vector<vertex> node_state_;
    bool is_root_;
    int vertexColored;    //The vertex colored in this node;Debug purpose
    CProxy_Node parent_;
    int uncolored_num_;   //number of uncolored vertex
    int child_num_;       //number of children this chare creates
                          //each element corresponds to a subgraph
    int child_finished_;  //current finished children
    int child_succeed_;
    bool is_and_node_;    //set to true if it requires all children reply

  public:
    // default constructor creats root node
    Node(bool isRoot, int n, CProxy_Node parent);
    Node(std::vector<vertex> state, bool isRoot,
            int n, int, CProxy_Node parent);
    Node (CkMigrateMessage*);

    // return the most constrained vertex id/index in vector
    int getNextConstraintVertex();

    // update a passed in state
    // by coloring vertex[vIndex] with color c
    void updateState(std::vector<vertex> & state, int vIndex, int c);

    // print out graph colored states
    void printGraph();

    // color the remaining part of graph locally
    // and respond to parent with result
    void colorLocally();

    // color remotely
    // choose a constreint vertex, color it
    // fire child chare to color remaining vertices
    void colorRemotely();

    // when receive its child response
    // current chare should decide how to merge with the received part
    // and how to respond to its parent
    // reutrn whether need to wait for another child or not
    bool mergeToParent(bool, std::vector<vertex>);

    //Checks if the reported coloring is valid. 
    bool isColoringValid(std::vector<vertex>);
};


#endif
