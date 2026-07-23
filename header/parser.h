#ifndef CGPL_PARSER_H_
#define CGPL_PARSER_H

#include "cfg.h"

typedef struct ParseTree {
    const Grammar* grammar;
    List_Node* node;
} ParseTree;

ParseTree* cgpl_parse(List_Node* head);

#endif