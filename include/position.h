#ifndef position_h_guard
#define position_h_guard

struct Position {

    int charNum;
    int lineNum;

    Position();
    Position(int c, int l);
    Position(const Position &obj);

};

#endif 