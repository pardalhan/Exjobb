#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>

using namespace cv;

namespace Data_generator {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btn_write;
	protected:
	private: System::Windows::Forms::Label^  lbl_writing;
	private: System::Windows::Forms::PictureBox^  pictureBox1;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btn_write = (gcnew System::Windows::Forms::Button());
			this->lbl_writing = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// btn_write
			// 
			this->btn_write->Location = System::Drawing::Point(294, 310);
			this->btn_write->Name = L"btn_write";
			this->btn_write->Size = System::Drawing::Size(107, 55);
			this->btn_write->TabIndex = 0;
			this->btn_write->Text = L"Write this";
			this->btn_write->UseVisualStyleBackColor = true;
			this->btn_write->Click += gcnew System::EventHandler(this, &MyForm::btn_write_Click);
			// 
			// lbl_writing
			// 
			this->lbl_writing->AutoSize = true;
			this->lbl_writing->Location = System::Drawing::Point(63, 54);
			this->lbl_writing->Name = L"lbl_writing";
			this->lbl_writing->Size = System::Drawing::Size(64, 13);
			this->lbl_writing->TabIndex = 1;
			this->lbl_writing->Text = L"Writing here";
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(211, 31);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(208, 250);
			this->pictureBox1->TabIndex = 2;
			this->pictureBox1->TabStop = false;
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(435, 394);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->lbl_writing);
			this->Controls->Add(this->btn_write);
			this->Name = L"MyForm";
			this->Text = L"Point and click";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}

#pragma endregion
	private: System::Void btn_write_Click(System::Object^  sender, System::EventArgs^  e) {
		lbl_writing->Text = "Funkar";

		Mat CVimg = cv::imread("D:\Testdata\\lena.png");

		pictureBox1->Width = CVimg.cols;
		pictureBox1->Height = CVimg.rows;
		pictureBox1->Image = gcnew System::Drawing::Bitmap(CVimg.cols, CVimg.rows);

		System::Drawing::Bitmap^ bmpImage = gcnew Bitmap(
			CVimg.cols, CVimg.rows, CVimg.step,
			System::Drawing::Imaging::PixelFormat::Format24bppRgb,
			System::IntPtr(CVimg.data)
			);

		pictureBox1->Image = bmpImage;
	

		namedWindow("MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
		imshow("MyWindow", CVimg); //display the image which is stored in the 'img' in the "MyWindow" window
		waitKey(0);
		destroyWindow("MyWindow"); //destroy the window with the name, "MyWindow"


	}
	};
}
