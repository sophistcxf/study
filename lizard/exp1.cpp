bool foo(int n) {
    if (n > 10) {
        if (n % 2 == 0) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

bool foo3(int n) {
    if (n % 2 == 0) {
        return true;
    }
    else {
        return false;
    }
}

bool foo2(int n) {
    if (n > 10) {
        return foo3(n);
    }
    else {
        return false;
    }
}
