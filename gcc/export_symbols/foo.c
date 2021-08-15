/* foo.c */
int foo() { return 42; }
int bar() { return foo() + 1; }
int baz() { return bar() - 1; }
