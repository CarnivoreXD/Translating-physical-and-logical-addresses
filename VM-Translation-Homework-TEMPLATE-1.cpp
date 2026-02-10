
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

ofstream result("./result", ios::out);
ifstream LogicalAddr("./logical_addr_input.txt", ios::in);
ifstream PhysicalAddr("./physical_addr_input.txt", ios::in);
// Initialize the page table and the page size
static const vector<int> PageTable = { 8192, 24576, 32768, -1, 28672, 4096, 16384, 20480 };
static const int PageSize = 4096;
vector<int> Logical(8);
vector<int> Physical(8);


/** *********************************************************************************
 * Translates 8 logical addresses that are specified in a file "logical_addr_input" *
 * Translates 8 physical addresses that are specified in a file "physical_addr_input"*
 ************************************************************************************/

void read_logical_addr_file()
{
    if (!LogicalAddr) cerr << "cannot open logical_addr_input file" << "\n";
    for (int i = 0; i < 9; i++) LogicalAddr >> Logical[i];
}

void read_physical_addr_file()
{
    if (!PhysicalAddr) cerr << "cannot open  physical_addr_input file" << "\n";
    for (int i = 0; i < 9; i++) PhysicalAddr >> Physical[i];
}



void translate_logical_to_physical()
{
    result << "\nTranslating from Logical to Physical Address" << endl;
    
    for (size_t i = 0; i < Logical.size(); ++i)
    {
        int index = Logical[i] / PageSize;
        int offset = Logical[i] % PageSize;
        int physicalAddr = offset + PageTable[index];

        if (PageTable[index] > 0)
        {
            
            result << "\nLogical Address: " <<Logical[i] << endl << "PageTableIndex = "<<index << 
            "   |   Offset: " <<offset << endl << "Corresponding Physical Address: " <<physicalAddr <<endl;
        }

        else
        {
            result << "\nLogical Address: " <<Logical[i] << endl << "PageTableIndex = "<<index << 
            "   |  Page Fault: Corresponding Physical Address Not in Memory"<<endl;
        }

    }
    
}

void translate_physical_to_logical()
{
    result << "\nTranslating from Physical to Logical Address" << endl;
    
    for (size_t i = 0; i < Physical.size(); ++i)
    {
        int pageFrame = Physical[i] / PageSize;
        int remainder = Physical[i] % PageSize;
        int physicalStartPhysAddr = pageFrame * PageSize;
        int logicalFrame = -1;


        result << "\nPhysical Address: " << Physical[i] << endl;


        for (size_t j = 0; j < PageTable.size(); ++j)
        {
            if(PageTable[j] == physicalStartPhysAddr)
            {
                logicalFrame = j;
                break;
            }
        }

        int logicalPage = logicalFrame * PageSize;
        int logicalAddress = logicalPage + remainder;

        if (logicalFrame >= 0)
        {
            
            result <<"PageFrame = "<<pageFrame <<"   |   PageTableIndex = " <<logicalFrame
                <<"   |   LogicalPage = " <<logicalPage<<" |  Offset = " <<remainder << endl
                    <<"Corresponding Logical Address: "<<logicalAddress << endl;
        }

        else
        {
            result <<"PageFrame = "<<pageFrame <<"   |   PageTableIndex = Not found"<<
                "   |   Page Fault: Corresponding Physical Address Not in Memory" << endl;
        }

        
    }
}

/** *********************************************
*  MAIN DRIVER CODE FOR TRANSLATING ADDRESSES  *
***********************************************/
int main() {
    // Print a Translation Header.
    result << "Page Table Entries: ";
    for (auto& tableEntry : PageTable) {
        result << tableEntry << " ";
    }
    result << "\nPage Size = " << PageSize << "\n";

    read_logical_addr_file();
    result << "\n******************************************\n";
    translate_logical_to_physical();
    result << endl;
    read_physical_addr_file();
    result << "\n******************************************\n";
    translate_physical_to_logical();
    result << endl;

}

