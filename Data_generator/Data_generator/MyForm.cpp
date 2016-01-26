#include "MyForm.h"


using namespace System;
using namespace System::Windows::Forms; 
[STAThread]


void main()
{
	// Needed for opening the window (Start)
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Data_generator::MyForm form;	//Creates a object of the class Myform 
	Application::Run(%form);
	// Needed for opening the window (END)


};