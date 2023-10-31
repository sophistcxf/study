#include <iostream>

typedef long data_t;

#define IDENT 0
#define OP +

typedef struct {
    long len;
    data_t* data;
} vec_rec, *vec_ptr;


long vec_length(vec_ptr v) {
    return v->len;
}

data_t* get_vec_start(vec_ptr v) {
    return v->data;
}

void combine2(vec_ptr v, data_t* dest) {
}

void combine3(vec_ptr v, data_t* dest) {
    long i;
    long length = vec_length(v);
    data_t* data = get_vec_start(v);
    *dest = IDENT;
    for (i = 0; i < length; ++i) {
        *dest = *dest OP data[i];
    }    
}

void combine4(vec_ptr v, data_t* dest) {
    long i;
    long length = vec_length(v);
    data_t* data = get_vec_start(v);
    data_t acc = IDENT;
    for (i = 0; i < length; ++i) {
	acc = acc OP data[i];
    }
    *dest = acc;
}

int main() {
    vec_rec v;
    v.len = 1024;
    v.data = new data_t[v.len];
    for (int i = 0; i < v.len; ++i) {
        v.data[i] = 1;
    }
    data_t dest;
    combine3(&v, &dest);
    data_t* dest2 = &v.data[v.len-1];
    combine3(&v, dest2);
    std::cout << dest << "\t" << *dest2 << std::endl;
    return 0;
}
