
#ifndef __MEMBER_TABLE_HXX__
#define __MEMBER_TABLE_HXX__

class MemberTable {
    public:
        MemberTable();
        ~MemberTable();
    public:
        void addMember();
        void declareDeadMember();
        void resetMemberTable();
    private:
        vector<MemberNode *> _memberList;
};

#endif

