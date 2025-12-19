#include <iostream>
#include "server.h"
#include <string.h>
#include <iomanip>

using std ::cin;
using std ::cout;
using std ::endl;
using std ::left;
using std ::setw;

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
    m_PrivateExpenses = 0;
    m_IndividualShare = 0;
    pNext = NULL;
    pPrev = NULL;
}

CSplitExpense ::~CSplitExpense()
{
    szName = "";
    m_AmountPaid = 0;
    m_BalancePending = 0;
    m_PrivateExpenses = 0;
    m_IndividualShare = 0;
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

    pNewNode->m_AmountPaid = 0;
    pNewNode->m_BalancePending = 0;
    pNewNode->m_IndividualShare = 0;
    pNewNode->m_PrivateExpenses = 0;
    pNewNode->pPrev = NULL;

    if (NULL == pFirst)
    {
        pNewNode->pNext = NULL;
        pFirst = pNewNode;
        cout << "Member Added Successfully!" << endl;
        goto CHOICE;
    }

    pNewNode->pNext = pFirst;
    pFirst->pPrev = pNewNode;
    pFirst = pNewNode;

    cout << "Member Added Successfully!" << endl;
    char chChoice;

CHOICE:
    cout << "Would you like to add another member? (y/n): ";
    cin >> chChoice;

    if (chChoice == 'y' || chChoice == 'Y')
    {
        AddNewMembers();
    }
    else
    {
        return;
    }
}

double CExpense ::RemoveAMember()
{
    cout << "Enter the name of the member : ";
    string szName;
    cin >> szName;

    if (NULL == pFirst)
    {
        cout << "No members found! Please add members first" << endl;
        return 0;
    }

    double iPendingBalance = 0;
    CSplitExpense *pTemp = pFirst;

    while (NULL != pTemp)
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
            char chChoice;
            double iAmount;

            cout << "Enter Amount Spend : ";
            cin >> iAmount;
            pTemp->m_AmountPaid += iAmount;
            cout << "Expense Added Successfully!" << endl;
            cout << "How would you like to split this expense\n1. Equally among all members\n2. Among selective members?\n";
            cout << "Enter your choice: ";
            cin >> chChoice;

            if (chChoice == '1')
            {
                SplitEqually(iAmount);
            }
            else if (chChoice == '2')
            {
                SplitAmongFew(iAmount);
            }
            else
            {
                cout << "Invalid Choice!" << endl;
                return;
            }
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
            double iAmount;
            cout << "Enter Amount to Delete : ";
            cin >> iAmount;
            if(iAmount > pTemp->m_AmountPaid)
            {
                cout << "Error: Amount to delete exceeds amount paid by member!" << endl;
                return;
            }
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

double CExpense ::CalculateTotalExpense()
{
    if (NULL == pFirst)
    {
        cout << "No Members Found! Please add members first." << endl;
        return 0;
    }

    double iTotalExpense = 0;
    CSplitExpense *pTemp = pFirst;
    while (NULL != pTemp)
    {
        iTotalExpense += pTemp->m_AmountPaid;
        pTemp = pTemp->pNext;
    }
    cout << "Total Expense is : " << iTotalExpense << endl;
    return iTotalExpense;
}

void CExpense ::SplitAmongFew(double iTotalExpense)
{
    if (iTotalExpense == 0)
    {
        return;
    }

    int iMembers = 0;

    cout << "Among how many members do you want to split the expense? ";
    cin >> iMembers;

    if(iMembers <= 0 || iMembers > MemberCount())
    {
        cout << "Invalid number of members!" << endl;
        return;
    }

    CSplitExpense *pTemp = pFirst;
    int iCount = 0;
    int iMemberCount = MemberCount();
    int arArray[iMembers];

    double iIndividualShare = iTotalExpense / iMembers;

    pTemp = pFirst;
    cout << "Choose the members to split expense: " << endl;
    for (iCount = 0; iCount < iMemberCount; iCount++)
    {
        cout << iCount + 1 << ". " << pTemp->szName << endl;
        pTemp = pTemp->pNext;
    }
    for (iCount = 0; iCount < iMembers; iCount++)
    {
        cout << "Enter " << iCount + 1 << "st member number: ";
        cin >> arArray[iCount];
    }
    int iIndex;
    for (iCount = 0; iCount < iMembers; iCount++)
    {
        iIndex = 1;
        pTemp = pFirst;
        while (iIndex != arArray[iCount])
        {
            iIndex++;
            pTemp = pTemp->pNext;
        }
        pTemp->m_PrivateExpenses += iIndividualShare;
    }
    cout << "Expense added successfully!" << endl;
}

void CExpense ::SplitEqually(double iTotalExpense)
{
    int iMemberCount = MemberCount();
    double iIndividualShare = iTotalExpense / iMemberCount;

    CSplitExpense *pTemp = pFirst;
    while (NULL != pTemp)
    {
        pTemp->m_IndividualShare += iIndividualShare;
        pTemp = pTemp->pNext;
    }
    cout << "Expense added successfully!" << endl;
}

void CExpense ::SplitExpense()
{
    if (NULL == pFirst)
    {
        return;
    }
    CSplitExpense *pTemp = pFirst;
    ClearPendingBalances();
    while (NULL != pTemp)
    {
        pTemp->m_BalancePending = pTemp->m_IndividualShare - pTemp->m_AmountPaid;
        pTemp = pTemp->pNext;
    }
    cout << "Expense Split Successfully!" << endl;
    PrintAllPendingBalances();
}

void CExpense ::ClearPendingBalances()
{
    if (NULL == pFirst)
    {
        return;
    }

    CSplitExpense *pTemp = pFirst;

    while (NULL != pTemp)
    {
        pTemp->m_BalancePending = 0;
        pTemp = pTemp->pNext;
    }
}

void CExpense ::PrintAllExpenses()
{
    CSplitExpense *pTemp = pFirst;
    cout << "********** Total Expenses **********" << endl;
    cout << left
         << setw(15) << "Name"
         << setw(15) << "AmountPaid"
         << setw(30) << "Amount spent by Individual"
         << setw(20) << "Amount to Pay" 
         << setw(15) << "Private Expenses"
         << setw(25) << "Total Individual Expense" << endl;
    while (NULL != pTemp)
    {
        cout << left
             << setw(15) << pTemp->szName
             << setw(15) << pTemp->m_AmountPaid
             << setw(30) << pTemp->m_IndividualShare
             << setw(20) << pTemp->m_BalancePending 
             << setw(15) << pTemp->m_PrivateExpenses
             << setw(25) << pTemp->m_IndividualShare + pTemp->m_PrivateExpenses << endl;

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

void CExpense ::PrintIndividualExpenses()
{
    CSplitExpense *pTemp = pFirst;
    cout << "********** Individual Expenses **********" << endl;
    while (NULL != pTemp)
    {
        cout << pTemp->szName << " : " << pTemp->m_IndividualShare + pTemp->m_PrivateExpenses << endl;
        pTemp = pTemp->pNext;
    }
}

double CExpense ::MemberCount()
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
        cout << " 6. Split Expenses " << endl;
        cout << " 7. Print All Expenses " << endl;
        cout << " 8. Print All Pending Balances " << endl;
        cout << " 9. Settle All Expenses " << endl;
        cout << " 0. Exit " << endl;
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
            PrintIndividualExpenses();
            break;
        case '9':
            SettleAllExpenses();
            break;
        case '0':
            cout << "Thank you for using SplitExpense!!! Exiting... " << endl;
            exit(0);
        default:
            cout << "Invalid Choice! Please try again... " << endl;
            break;
        }
    }
}

void CExpense ::SettleAllExpenses()
{
    // SplitExpense();
    if (NULL == pFirst || pFirst->pNext == NULL)
    {
        return;
    }

    CSplitExpense *pTemp1 = pFirst;
    CSplitExpense *pTemp2 = pFirst;
    CSplitExpense *pTemp = NULL;

    cout << "\n*********** FINAL SPLIT *************" << endl;

    while (1)
    {
        if (pTemp1->m_BalancePending > 0) // first positive
        {
            while (pTemp2->m_BalancePending != 0)
            {
                pTemp2 = pTemp2->pNext;
                if (NULL == pTemp2)
                    break;
            }

            if (pTemp2 == NULL)
                break;

            while (pTemp2->m_BalancePending < 0)
            {

                if (pTemp1->m_BalancePending + pTemp2->m_BalancePending < 0)
                {
                    cout << pTemp1->szName << " pays " << pTemp1->m_BalancePending << " to " << pTemp2->szName << endl;
                    pTemp2->m_BalancePending += pTemp1->m_BalancePending;
                    pTemp1->m_BalancePending = 0;
                    while (pTemp1->m_BalancePending > 0)
                        pTemp1 = pTemp1->pNext;
                    // if(NULL == pTemp1)
                    //     break;
                    if (pTemp1->m_BalancePending == 0)
                        break;
                }
                else if (pTemp2->m_BalancePending + pTemp1->m_BalancePending >= 0)
                {
                    cout << pTemp1->szName << " pays " << -pTemp2->m_BalancePending << " to " << pTemp2->szName << endl;
                    pTemp1->m_BalancePending += pTemp2->m_BalancePending;
                    pTemp2->m_BalancePending = 0;
                    break;
                }

            }
        }
        if (pTemp1->m_BalancePending <= 0)
            pTemp1 = pTemp1->pNext; // first negative or zero

        pTemp = pFirst;
        while (NULL != pTemp)
        {
            if (pTemp->m_BalancePending != 0)
                break;
            pTemp = pTemp->pNext;
        }
        if (NULL == pTemp)
            break;
    }
}