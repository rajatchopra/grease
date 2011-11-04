
#ifndef __MEMBER_TABLE_HXX__
#define __MEMBER_TABLE_HXX__

#include <map>
#include <string>


class MemberTable {
    public:
        MemberTable();
        ~MemberTable();
    public:
        void addMember();
        void declareDeadMember();
        void resetMemberTable();
    private:
        map<MemberNode *, string> _memberList;
};

#endif

