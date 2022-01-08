#include "position.h"

Position::Position() {

    charNum = 0;
    lineNum = 0;

}

Position::Position(const Position &obj) {

    charNum = obj.charNum;
    lineNum = obj.lineNum;

}

Position::Position(int c, int l) {

    charNum = c;
    lineNum = l;

}