struct People {
    1:string name,
    2:i16 age,
    3:bool male
}

typedef list<People> Peoples;

struct Nation {
    1: list<People> peoples
}

exception Exception {
    1:string message
}

service Service {
    void fun1(1:i32 a, 2:string b),
    string fun2() throws (1:Exception e)
    i32 add(1:i32 a, 2:i32 b)
}
