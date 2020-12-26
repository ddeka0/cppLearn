##### https://stackoverflow.com/questions/3106110/what-is-move-semantics and https://stackoverflow.com/questions/3413470/what-is-stdmove-and-when-should-it-be-used

##### m3.cpp was written to track the memory allocation along with move semantics


     valgrind --leak-check=full --soname-synonyms=somalloc=nouserintercepts ./a.out 
    ==25841== Memcheck, a memory error detector
    ==25841== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
    ==25841== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
    ==25841== Command: ./a.out
    ==25841== 
    125 => Op : Int a(1)
    51 Int::Int(int)
    This object : 0x1ffefffb38
    User Defined :: Operator new 4 0x5bca0c0
    55 allocated plain int 0x5bca0c0


    129 => Op : Array arr
    114 Array::Array()


    133 => Op : arr.m_data[0] = std::move(a);
    User Defined :: Operator new 48 0x5bca110
    40 Int::Int()
    95 Int& Int::operator=(Int&&)


    138 => Op : Int b(2);
    51 Int::Int(int)
    This object : 0x1ffefffb40
    User Defined :: Operator new 4 0x5bca180
    55 allocated plain int 0x5bca180


    143 => Op : arr.m_data[1] = std::move(b);
    User Defined :: Operator new 48 0x5bca1d0
    40 Int::Int()
    95 Int& Int::operator=(Int&&)


    148 => Op : arr.m_data[0] = Int(10);
    51 Int::Int(int)
    This object : 0x1ffefffb48
    User Defined :: Operator new 4 0x5bca240
    55 allocated plain int 0x5bca240
    95 Int& Int::operator=(Int&&)
    98 deleted plain int 0x5bca0c0
    User Defined :: Operator delete 0x5bca0c0
    42 Int::~Int()
    This object : 0x1ffefffb48


    154 => Op : arr.m_data[0] = 10;
    51 Int::Int(int)
    This object : 0x1ffefffb48
    User Defined :: Operator new 4 0x5bca290
    55 allocated plain int 0x5bca290
    95 Int& Int::operator=(Int&&)
    98 deleted plain int 0x5bca240
    User Defined :: Operator delete 0x5bca240
    42 Int::~Int()
    This object : 0x1ffefffb48


    42 Int::~Int()
    This object : 0x1ffefffb40
    117 Array::~Array()
    42 Int::~Int()
    This object : 0x5bca1f8
    45 deleted plain int 0x5bca180
    User Defined :: Operator delete 0x5bca180
    User Defined :: Operator delete 0x5bca1d0
    42 Int::~Int()
    This object : 0x5bca138
    45 deleted plain int 0x5bca290
    User Defined :: Operator delete 0x5bca290
    User Defined :: Operator delete 0x5bca110
    42 Int::~Int()
    This object : 0x1ffefffb38
    ==25841== 
    ==25841== HEAP SUMMARY:
    ==25841==     in use at exit: 0 bytes in 0 blocks
    ==25841==   total heap usage: 8 allocs, 8 frees, 73,840 bytes allocated
    ==25841== 
    ==25841== All heap blocks were freed -- no leaks are possible
    ==25841== 
    ==25841== For counts of detected and suppressed errors, rerun with: -v
    ==25841== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
