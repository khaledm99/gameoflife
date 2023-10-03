#pragma once
// Helper function to compute a mod n,
// including the case where a is negative.
// C++ built in mod() function doesn't handle
// the negative case
inline int mod_floor(int a, int n) {
    if(a<0) return n+a;
    else if(a==n) return 0;
    else if(a>n) return mod_floor(a-n, n);
    else return a;
    
}

// Helper function to round integer division
// to the nearest integer. C++ default behaviour is
// to truncate downwards.
inline int find_nearest(unsigned int a, unsigned int b) {
    if(b==0) throw "divide by zero";
    return (a + (b/2)) / b;
    //return round((double)a/(double)b);
}
