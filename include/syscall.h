void init_syscall();

#define DECL_SC0(fn) int syscall_##fn();
#define DECL_SC1(fn,p1) int syscall_##fn(p1);
#define DECL_SC2(fn,p1,p2) int syscall_##fn(p1,p2);
#define DECL_SC3(fn,p1,p2,p3) int syscall_##fn(p1,p2,p3);
#define DECL_SC4(fn,p1,p2,p3,p4) int syscall_##fn(p1,p2,p3,p4);
#define DECL_SC5(fn,p1,p2,p3,p4,p5) int syscall_##fn(p1,p2,p3,p4,p5);

#define DEFN_SC0(fn,num)\
int syscall_##fn()\
{\
    int a;\
    asm volatile("int $0x80":"=a"(a):"0"(num));\
    return a;\
}
#define DEFN_SC1(fn,num,P1)\
int syscall_##fn(P1 p1)\
{\
    int a;\
    asm volatile("int $0x80":"=a"(a):"0"(num),"b"((int)p1));\
    return a;\
}
#define DEFN_SC2(fn,num,P1,P2)\
int syscall_##fn(P1 p1,P2 p2)\
{\
    int a;\
    asm volatile("int $0x80":"=a"(a):"0"(num),"b"((int)p1),"c"((int)p2));\
    return a;\
}

DECL_SC1(printstring,const char*)
