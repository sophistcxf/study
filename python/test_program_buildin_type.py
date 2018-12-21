def add(a, b):
    '''
    add a and b
    '''
    return a + b

def function_buildin_type():
    print "add.__doc__"
    print add.__doc__
    print "add.__name__"
    print add.__name__
    print "add.__dict__"
    print add.__dict__
    print "add.__code__"
    print add.__code__
    print "add.__defaults__"
    print add.__defaults__
    print "add.__globals__"
    print add.__globals__
    print "add.__closure__"
    print add.__closure__

if __name__ == "__main__":
    function_buildin_type()
