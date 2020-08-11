#ifndef NODE_H
#define NODE_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/* Node datatype and functions */

struct Node {
	// the tag is the node type (for parse trees, the grammar symbol)
	int tag;
	// dynamic array of pointers to child nodes
	struct Node **kids;
	// how many children there are
	int num_kids;
	// total capacity of kids array
	int capacity;

	// payload data
	char *str;
	long ival;

	// information about source file and location
	const char *srcfile;
	int line, col;
};

struct Node *node_alloc(int tag, int initial_capacity);
struct Node *node_alloc_str_copy(int tag, const char *str_to_copy);
struct Node *node_alloc_str_adopt(int tag, char *str_to_adopt);
struct Node *node_alloc_ival(int tag, long ival);
struct Node *node_build0(int tag);
struct Node *node_build1(int tag, struct Node *child1);
struct Node *node_build2(int tag, struct Node *child1, struct Node *child2);
struct Node *node_build3(int tag, struct Node *child1, struct Node *child2, struct Node *child3);
struct Node *node_build4(int tag,
                         struct Node *child1, struct Node *child2, struct Node *child3,
                         struct Node *child4);
struct Node *node_build5(int tag,
                         struct Node *child1, struct Node *child2, struct Node *child3,
                         struct Node *child4, struct Node *child5);
struct Node *node_build6(int tag,
                         struct Node *child1, struct Node *child2, struct Node *child3,
                         struct Node *child4, struct Node *child5, struct Node *child6);
struct Node *node_build7(int tag,
                         struct Node *child1, struct Node *child2, struct Node *child3,
                         struct Node *child4, struct Node *child5, struct Node *child6,
                         struct Node *child7);
struct Node *node_build8(int tag,
                         struct Node *child1, struct Node *child2, struct Node *child3,
                         struct Node *child4, struct Node *child5, struct Node *child6,
                         struct Node *child7, struct Node *child8);
struct Node *node_buildn(int tag, ...);
void node_destroy(struct Node *n);
void node_destroy_recursive(struct Node *n);
int node_get_tag(struct Node *n);
int node_get_num_kids(struct Node *n);
void node_add_kid(struct Node *n, struct Node *kid);
struct Node *node_get_kid(struct Node *n, int index);
int node_first_kid_has_tag(struct Node *n, int tag);

void node_set_source_info(struct Node *n, const char *srcfile, int line, int col);

const char *node_get_str(struct Node *n);
long node_get_ival(struct Node *n);
void node_set_ival(struct Node *n, long ival);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // NODE_H
