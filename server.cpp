#include <iostream>
#include "server.h"
#include "string.h"

using std ::cin;
using std ::cout;
using std ::endl;

CExpense ::CExpense()
{
    pFirst = NULL;
}

CExpense ::~CExpense()
{
    pFirst = NULL;
}

CSplitExpense ::CSplitExpense()
{
    szName = "";
    m_AmountPaid = 0;
    m_BalancePending = 0;
    pNext = NULL;
    pPrev = NULL;
}

CSplitExpense ::~CSplitExpense()
{
    szName = "";
    m_AmountPaid = 0;
    m_BalancePending = 0;
    pNext = NULL;
    pPrev = NULL;
}

void CExpense ::AddNewMembers() // Insert at Beginning
{
    CSplitExpense *pNewNode = new CSplitExpense;

    if (NULL == pNewNode)
    {
        cout << "Memory Allocation Failed\n"
             << endl;
        return;
    }

    cout << "Enter New Member Name : ";
    cin >> pNewNode->szName;

    cout << "Enter Amount Paid by " << pNewNode->szName << " : ";
    cin >> pNewNode->m_AmountPaid;

    pNewNode->m_BalancePending = 0;
    pNewNode->pPrev = NULL;

    if (NULL == pFirst)
    {
        pNewNode->pNext = NULL;
        pFirst = pNewNode;
        cout << "Member Added Successfully!" << endl;
        return;
    }

    pNewNode->pNext = pFirst;
    pFirst->pPrev = pNewNode;
    pFirst = pNewNode;

    cout << "Member Added Successfully!" << endl;
}

int CExpense ::RemoveAMember()
{
    cout << "Enter the name of the member : ";
    string szName;
    cin >> szName;

    if (NULL == pFirst)
    {
        return 0;
    }

    int iPendingBalance = 0;
    CSplitExpense *pTemp = pFirst;

    while (NULL != pTemp->pNext)
    {
        if (pTemp->szName == szName)
        {
            iPendingBalance = pTemp->m_BalancePending;
            if (pTemp->pPrev == NULL) // first node
            {
                pFirst = pTemp->pNext;
                pFirst->pPrev = NULL;
            }
            else if (pTemp->pNext == NULL) // last node
            {
                pTemp->pPrev->pNext = NULL;
            }
            else // middle node
            {
                pTemp->pPrev->pNext = pTemp->pNext;
                pTemp->pNext->pPrev = pTemp->pPrev;
            }

            pTemp->pNext = NULL;
            pTemp->pPrev = NULL;
            delete pTemp;
            pTemp = NULL;
            cout << "Member Removed Successfully!" << endl;
            return iPendingBalance;
        }
        pTemp = pTemp->pNext;
    }
    cout << "Member not found!" << endl;
    return 0;
}

void CExpense ::AddAnExpense()
{
    if (NULL == pFirst)
    {
        cout << "No Members Found! Please add members first." << endl;
        return;
    }

    string szName;
    CSplitExpense *pTemp = NULL;

ADDAGAIN:
    cout << "Amount spent by : ";
    cin >> szName;
    pTemp = pFirst;
    while (NULL != pTemp)
    {
        if (pTemp->szName == szName)
        {
            cout << "Enter Amount Spend : ";
            int iAmount;
            cin >> iAmount;
            pTemp->m_AmountPaid += iAmount;
            cout << "Expense Added Successfully!" << endl;
            return;
        }
        else
        {
            pTemp = pTemp->pNext;
        }
    }
    cout << "Member not found!" << endl;
    cout << "Try Again!" << endl;
    goto ADDAGAIN;
}

void CExpense ::DeleteExpense()
{
    if (NULL == pFirst)
    {
        cout << "No Members Found! Please add members first." << endl;
        return;
    }
    string szName;
    CSplitExpense *pTemp = pFirst;

    cout << "Enter the name of the member to delete expense : ";
    cin >> szName;

    while (NULL != pTemp)
    {
        if (pTemp->szName == szName)
        {
            int iAmount;
            cout << "Enter Amount to Delete : ";
            cin >> iAmount;
            pTemp->m_AmountPaid = pTemp->m_AmountPaid - iAmount;
            cout << "Expense Deleted Successfully!" << endl;
            return;
        }
        else
        {
            pTemp = pTemp->pNext;
        }
    }
    cout << "Member not found!" << endl;
}

int CExpense ::CalculateTotalExpense()
{
    if (NULL == pFirst)
    {
        cout << "No Members Found! Please add members first." << endl;
        return 0;
    }

    int iTotalExpense = 0;
    CSplitExpense *pTemp = pFirst;
    while (NULL != pTemp)
    {
        iTotalExpense += pTemp->m_AmountPaid;
        pTemp = pTemp->pNext;
    }
    cout << "Total Expense is : " << iTotalExpense << endl;
    return iTotalExpense;
}

void CExpense ::SplitExpense()
{
    cout << "In Splitting Expense..." << endl;
    int iTotalExpense = CalculateTotalExpense();

    if (iTotalExpense == 0)
    {
        return;
    }

    cout << "How do you want to split expenses?\n1. Split equally among all members\n2. Split among a few" << endl;

    char chChoice;
    cout << "Enter your choice : ";
    cin >> chChoice;

    int iMemberCount = 0;

    switch (chChoice)
    {
    case '1':
        iMemberCount = MemberCount();
        break;
    case '2':
        cout << "Enter number of members to split among : ";
        cin >> iMemberCount;

        if (iMemberCount > MemberCount() || iMemberCount <= 0)
        {
            cout << "Invalid number of members! Returning to Main Menu..." << endl;
            return;
        }
        break;
    default:
        cout << "Invalid Choice! Returning to Main Menu..." << endl;
        return;
    }

    int iIndividualShare = iTotalExpense / iMemberCount;
    CSplitExpense *pTemp = pFirst;

    if (chChoice == '1')
    {
        while (NULL != pTemp)
        {
            pTemp->m_BalancePending = iIndividualShare - pTemp->m_AmountPaid;
            pTemp = pTemp->pNext;
        }
    }
    else
    {
        int iCount = 0;
        int iMembers = MemberCount();
        int arArray[iMemberCount];
        pTemp = pFirst;
        ClearPendingBalances();
        cout << "Choose the members to split expense: " << endl;
        for (iCount = 0; iCount < iMembers; iCount++)
        {
            cout << iCount + 1 << ". " << pTemp->szName << endl;
            pTemp = pTemp->pNext;
        }
        for (iCount = 0; iCount < iMemberCount; iCount++)
        {
            cout << "Enter " << iCount + 1 << "st member number: ";
            cin >> arArray[iCount];
        }
        int iIndex;
        for(iCount = 0; iCount < iMemberCount; iCount++)
        {
            iIndex = 1;
            pTemp = pFirst;
            while(iIndex != arArray[iCount])
            {
                iIndex++;
                pTemp = pTemp->pNext;
            }
            pTemp->m_BalancePending = iIndividualShare - pTemp->m_AmountPaid;
        }
    }
}

void CExpense ::SplitEqually(int iTotalExpense)
{
    int iMemberCount = MemberCount();
    int iIndividualShare = iTotalExpense / iMemberCount;

    CSplitExpense *pTemp = pFirst;
    while (NULL != pTemp)
    {
        pTemp->m_BalancePending = iIndividualShare - pTemp->m_AmountPaid;
        pTemp = pTemp->pNext;
    }
}

void CExpense :: ClearPendingBalances()
{
    if(NULL == pFirst)
    {
        return;
    }

    CSplitExpense *pTemp = pFirst;

    while(NULL != pTemp)
    {
        pTemp->m_BalancePending = 0;
        pTemp = pTemp->pNext;
    }
}


void CExpense ::PrintAllExpenses()
{
    CSplitExpense *pTemp = pFirst;
    cout << "********** Total Expenses **********" << endl;
    while (NULL != pTemp)
    {
        cout << pTemp->szName << " : " << pTemp->m_AmountPaid << endl;
        pTemp = pTemp->pNext;
    }
}

void CExpense ::PrintAllPendingBalances()
{
    CSplitExpense *pTemp = pFirst;
    cout << "********** Pending Balances **********" << endl;
    while (NULL != pTemp)
    {
        cout << pTemp->szName << " : " << pTemp->m_BalancePending << endl;
        pTemp = pTemp->pNext;
    }
}

int CExpense ::MemberCount()
{
    if (NULL == pFirst)
    {
        return 0;
    }

    int iCount = 0;

    CSplitExpense *pTemp = pFirst;
    while (NULL != pTemp)
    {
        iCount++;
        pTemp = pTemp->pNext;
    }
    return iCount;
}

void CExpense ::DisplayMenu()
{
    char chChoice;
    while (1)
    {
        cout << " \n********** Expense Splitter Menu ********** " << endl;
        cout << " 1. Add New Member " << endl;
        cout << " 2. Remove a Member " << endl;
        cout << " 3. Add an Expense " << endl;
        cout << " 4. Delete an Expense " << endl;
        cout << " 5. Calculate Total Expense " << endl;
        cout << " 6. Split Expense " << endl;
        cout << " 7. Print All Expenses " << endl;
        cout << " 8. Print All Pending Balances " << endl;
        cout << " 9. Exit " << endl;
        cout << " Enter your choice : ";
        cin >> chChoice;

        switch (chChoice)
        {
        case '1':
            AddNewMembers();
            break;
        case '2':
            RemoveAMember();
            break;
        case '3':
            AddAnExpense();
            break;
        case '4':
            DeleteExpense();
            break;
        case '5':
            CalculateTotalExpense();
            break;
        case '6':
            SplitExpense();
            break;
        case '7':
            PrintAllExpenses();
            break;
        case '8':
            PrintAllPendingBalances();
            break;
        case '9':
            cout << "Thank you for using SplitExpense!!! Exiting... " << endl;
            exit(0);
        default:
            cout << "Invalid Choice! Please try again... " << endl;
            break;
        }
    }
}