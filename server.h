#pragma once
#include <iostream>
#include <string>

using std :: string;

class CExpense;

class CSplitExpense
{
private :
    string szName;
    double m_AmountPaid;
    double m_BalancePending;
    double m_IndividualShare;              // version 2
    double m_PrivateExpenses;              // version 2
    CSplitExpense *pNext;
    CSplitExpense *pPrev;

public :
    CSplitExpense();
    ~CSplitExpense();

    friend class CExpense;
};

class CExpense
{
private :
    CSplitExpense *pFirst;

public :
    CExpense();
    ~CExpense();

    void DisplayMenu();
    void AddNewMembers();
    double RemoveAMember();            // will return the pending balance of removed member
    void AddAnExpense();
    void DeleteExpense();
    double CalculateTotalExpense();
    void SplitExpense();            // old version
    void SplitAmongFew(double);            // new version
    void PrintAllExpenses();
    double MemberCount();
    void SplitEqually(double);
    void PrintAllPendingBalances();
    void ClearPendingBalances();
    void PrintIndividualExpenses();     // new version
    void SettleAllExpenses();            // new version
};
