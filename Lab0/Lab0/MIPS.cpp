//Authored by Chris Feng
//11/1/2019

#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.


class RF
{
    public:
        bitset<32> ReadData1, ReadData2; 
     	RF()
    	{ 
          Registers.resize(32);  
          Registers[0] = bitset<32> (0);  
        }
	
        void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
        {   
            // implement the funciton by you.
            if(WrtEnable ==1)
            {
                Registers[WrtReg.to_ulong()] = WrtData;
            }
            else{
                ReadData1 = Registers[RdReg1.to_ulong()];
                ReadData2 = Registers[RdReg2.to_ulong()];
            }
         }
		 
	void OutputRF() // write RF results to file
             {
               ofstream rfout;
                  rfout.open("/Users/fater/study/CSA/Lab0/RFresult.txt",std::ios_base::app);
                  if (rfout.is_open())
                  {
                    rfout<<"A state of RF:"<<endl;
                  for (int j = 0; j<32; j++)
                      {        
                        rfout << Registers[j]<<endl;
                      }
                     
                  }
                  else cout<<"Unable to open file";
                  rfout.close();
               
               }     
	private:
            vector<bitset<32> >Registers;
	
};

class ALU
{
      public:
             bitset<32> ALUresult;
             bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> oprand1, bitset<32> oprand2)
             {   
                 // implement the ALU operations by you.
                 int result ;
                 cout<<"这次计算"<<ALUOP<<endl;
                 switch (ALUOP.to_ulong()) {
                     case ADDU: result =oprand1.to_ulong() + oprand2.to_ulong();break;
                     case SUBU: result =oprand1.to_ulong() - oprand2.to_ulong();break;
                     case AND: result =oprand1.to_ulong() & oprand2.to_ulong();break;
                     case NOR: result =~(oprand1.to_ulong() | oprand2.to_ulong());break;
                     case OR: result =oprand1.to_ulong() | oprand2.to_ulong();break;
                 }
                 ALUresult = bitset<32>(result);
                 return ALUresult;
               }            
};

class INSMem
{
      public:
          bitset<32> Instruction;
          INSMem() // read instruction memory
          {       IMem.resize(MemSize); 
                  ifstream imem;
                  string line;
                  int i=0;
                  imem.open("/Users/fater/study/CSA/Lab0/test_cases/all instructions/imem.txt");
                  if (imem.is_open())
                  {
                  while (getline(imem,line))
                     {
                         IMem[i] = bitset<8>(std::stoi(line,nullptr,2));
                        i++;
                     }
                     
                  }
                  else cout<<"Unable to open file";
                  imem.close();
                     
                  }
                  
          bitset<32> ReadMemory (bitset<32> ReadAddress) 
              {    
               // implement by you. (Read the byte at the ReadAddress and the following three byte).
                  string ins ;
                  ins.append(IMem[ReadAddress.to_ulong()].to_string());
                  ins.append(IMem[ReadAddress.to_ulong()+1].to_string());
                  ins.append(IMem[ReadAddress.to_ulong()+2].to_string());
                  ins.append(IMem[ReadAddress.to_ulong()+3].to_string());
                  Instruction =bitset<32>(ins);
//                  cout<<"Current Instruction is"<<ins<<endl;
               return Instruction;
              }     
      
      private:
           vector<bitset<8> > IMem;
      
};
      
class DataMem    
{
      public:
          bitset<32> readdata;  
          DataMem() // read data memory
          {
             DMem.resize(MemSize); 
             ifstream dmem;
                  string line;
                  int i=0;
                  dmem.open("/Users/fater/study/CSA/Lab0/test_cases/all instructions/dmem.txt");
                  if (dmem.is_open())
                  {
                  while (getline(dmem,line))
                       {      
                           DMem[i] = bitset<8>(std::stoi(line,nullptr,2));
                        i++;
                       }
                  }
                  else cout<<"Unable to open file";
                  dmem.close();
          
          }  
          bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem) 
          {    
                if (writemem.to_ulong() == 1 && readmem.to_ulong() ==1)
                {
                    cout<<"cant write and read at the same time"<<endl;
                }
                if (writemem.to_ulong() == 1 )
                {
                    DMem[Address.to_ulong()] = bitset<8>(WriteData.to_string().substr(0,8));
                    DMem[Address.to_ulong()+1] = bitset<8>(WriteData.to_string().substr(8,8));
                    DMem[Address.to_ulong()+2] = bitset<8>(WriteData.to_string().substr(16,8));
                    DMem[Address.to_ulong()+3] = bitset<8>(WriteData.to_string().substr(24,8));
                }
                else if(readmem.to_ulong() == 1)
                {
                    string data;
                    data.append(DMem[Address.to_ulong()].to_string());
                    data.append(DMem[Address.to_ulong()+1].to_string());
                    data.append(DMem[Address.to_ulong()+2].to_string());
                    data.append(DMem[Address.to_ulong()+3].to_string());
                    readdata = bitset<32>(data);
                }
               // implement by you.
                return readdata;
          }   
                     
          void OutputDataMem()  // write dmem results to file
          {
               ofstream dmemout;
                  dmemout.open("/Users/fater/study/CSA/Lab0/dmemresult.txt");
                  if (dmemout.is_open())
                  {
                  for (int j = 0; j< 1000; j++)
                       {     
                        dmemout << DMem[j]<<endl;
                       }
                     
                  }
                  else cout<<"Unable to open file";
                  dmemout.close();
               
               }             
      
      private:
           vector<bitset<8> > DMem;
      
};  

unsigned long shiftbits(bitset<32> inst,int loc)
{
    return (inst.to_ulong() >> loc);
}

bitset<32> signextend(bitset<16> imm)
{
    string sestring;
    //判断符号位;
    if (imm[15]==0)
    {
        sestring = "0000000000000000" + imm.to_string();
    }else
    {
        sestring = "1111111111111111" + imm.to_string();
    }
    return (bitset<32>(sestring));
}

int main()
{
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;

    bitset<32> PC = 0;
    bitset<32> pcplus4;
    bitset<32> instruction;
    bitset<6> opcode;
    bitset<5> RReg1;
    bitset<5> RReg2;
    bitset<5> WReg;
    bitset<3> Aluop;
    bitset<5> shift_amount;
    bitset<16> imm;
    
    bitset<32> ReadData1;
    bitset<32> ReadData2;
    bitset<32> Aluoprand1;
    bitset<32> Aluoprand2;
    bitset<32> Aluresult;
    bitset<32> DMemData;
    bitset<32> DMemAddr;
    bitset<32> RFData;
    bitset<32> Jaddr;
    bitset<32> Baddr;
    bitset<1> isLoad;
    bitset<1> isStore;
    bitset<1> isEqual;
    bitset<1> is_I_type;
    bitset<1> is_R_type;
    bitset<1> is_J_type;
    bitset<1> is_Branch;
    bitset<1> Wrtenable;
    
    while (1)
	{
        // Fetch
        cout<<"current PC="<<PC.to_ulong()<<endl;
        instruction = myInsMem.ReadMemory(PC);
        cout<<"current instuciton= "<<instruction<<endl;
		// If current insturciton is "11111111111111111111111111111111", then break;
        if (instruction.to_string() == "11111111111111111111111111111111")
        {
            cout<<"this is halt instruction"<<endl;
            break;
        }
		// decode(Read RF)
        opcode = bitset<6>(shiftbits(instruction,26));
        cout<<"opcode="<<opcode<<endl;
        
        is_I_type = (opcode.to_ulong()!= 0 && opcode.to_ulong() != 2)? 1:0;
        cout<<"is_I_type="<<is_I_type<<endl;
        is_R_type = (opcode.to_ulong() == 0)? 1:0;
        cout<<"is_R_type="<<is_R_type<<endl;
        is_J_type = (opcode.to_ulong() == 2)? 1:0;
        cout<<"is_J_type="<<is_J_type<<endl;
        is_Branch = (opcode.to_ulong() == 4)? 1:0;
        cout<<"is_Branch="<<is_Branch<<endl;
        isLoad = (opcode.to_ulong()==35)?1:0;//lw instruction
        cout<<"isLoad="<<isLoad<<endl;
        isStore = (opcode.to_ulong()==43)?1:0;
        cout<<"isStore="<<isStore<<endl;

        if (is_R_type.to_ulong())
        {
            RReg1 = bitset<5>(shiftbits(instruction, 21));
            RReg2 = bitset<5>(shiftbits(instruction, 16));
            WReg = bitset<5>(shiftbits(instruction, 11));
            shift_amount = bitset<5>(shiftbits(instruction,6));
        }
        if (is_I_type.to_ulong())
        {
            RReg1 = bitset<5>(shiftbits(instruction, 21));
            RReg2 = bitset<5>(shiftbits(instruction, 16));
            WReg = bitset<5>(shiftbits(instruction, 16));
            imm = bitset<16>(shiftbits(instruction, 0));
        }
        cout<<"RReg1="<<RReg1<<endl;
        cout<<"RReg2="<<RReg2<<endl;
        cout<<"WReg="<<WReg<<endl;
        if (isStore.to_ulong()|| is_Branch.to_ulong() || is_J_type.to_ulong())
            Wrtenable =0;
        else
            Wrtenable = 1;
        cout<<"Wrtenable="<<Wrtenable<<endl;
        if (isLoad.to_ulong() ||isStore.to_ulong())
            Aluop = bitset<3>("001");
        else if (is_R_type.to_ulong())
            Aluop = bitset<3>(shiftbits(instruction, 0));
        else Aluop = bitset<3>(shiftbits(instruction, 26));
        cout<<"Aluop:"<<Aluop<<endl;
        
        myRF.ReadWrite(RReg1, RReg2, WReg, 0,0);
        ReadData1 = myRF.ReadData1;
        ReadData2 = myRF.ReadData2;
        isEqual = (ReadData1 == ReadData2)? 1:0;
        cout<<"ReadData1="<<ReadData1<<",ReadData2="<<ReadData2<<endl;
		// Execute
        Aluoprand1 = ReadData1;
        Aluoprand2 = (is_I_type.to_ulong())? signextend(imm):ReadData2;
        Aluresult = myALU.ALUOperation(Aluop, Aluoprand1, Aluoprand2);
        cout<<"Aluoprand1="<<Aluoprand1<<",Aluoprand2"<<Aluoprand2<<endl;
        cout<<"Aluresult=" <<Aluresult <<endl;
		// Read/Write Mem
        DMemData = ReadData2;
        DMemAddr = Aluresult;
        cout<<"DMemData=" <<DMemData<<endl;
        cout<<"DMemAddr=" <<DMemAddr <<endl;
        cout<<isStore.to_ulong()<<endl;
        myDataMem.MemoryAccess(DMemAddr, DMemData, isLoad, isStore);
		// Write back to RF
        RFData =(isLoad.to_ulong())? myDataMem.readdata:Aluresult;
        myRF.ReadWrite(RReg1, RReg2, WReg, RFData, Wrtenable);
        cout<<"RFData="<<RFData<<endl;
        
        //normal
        pcplus4 = bitset<32>(PC.to_ulong() + 4);
        
        //J-type
        Jaddr = bitset<32>(bitset<4>(shiftbits(pcplus4,28)).to_string()+bitset<26>(shiftbits(instruction, 0)).to_string() + "00");
        cout<<"Jaddr:"<<Jaddr<<endl;
        //BEQ;
        Baddr = bitset<32>(PC.to_ulong()+4+bitset<32>(bitset<30>(shiftbits(signextend(imm),0)).to_string()+"00").to_ulong());
        
        PC = (is_Branch.to_ulong()&&isEqual.to_ulong())?Baddr:((is_J_type.to_ulong())? Jaddr:pcplus4);
        if(is_Branch.to_ulong()&&isEqual.to_ulong())
        {
            cout<<"branch to" <<Baddr<<endl;
        }
        myRF.OutputRF(); // dump RF;    
    }
        myDataMem.OutputDataMem(); // dump data mem
      
        return 0;
        
}
