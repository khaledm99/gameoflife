#ifndef UTIL_H
#define UTIL_H
inline int mod_floor(int a, int n) {
    if(a<0) return n-1;
    else if(a==n) return 0;
    else return a;
    
}
inline int find_nearest(int a, int b) {
    return (a + (b/2)) / b;
}
#endif
