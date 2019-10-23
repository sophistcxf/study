define add
    print $arg0 + $arg1 + $arg2
end

define adder2
    if $argc == 2
        print $arg0 + $arg1
    end
    if $argc == 3
        print $arg0 + $arg1 + $arg2
    end
end

define adder3
    set $i = 0
    set $sum = 0
    while $i < $argc
        eval "set $sum = $sum + $arg%d", $i
        set $i = $i + 1
    end
    print $sum
end

define test_set
    if $argc > 0
        set $i = $arg0
    else
        set $i = 10
    end
    print $i
end

define test_eval
    eval "set $i = 10"
    eval "set $i2 = %d", 20
    print $i
    print $i2
end
