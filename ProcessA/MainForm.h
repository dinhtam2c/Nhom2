#pragma once
#include <Windows.h>

void SendFile(LPCWSTR filePath);

namespace ProcessA {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
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
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^ txbPath;
	protected:

	protected:
	private: System::Windows::Forms::Button^ btnBrowse;
	private: System::Windows::Forms::Button^ btnSend;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MainForm::typeid));
			this->txbPath = (gcnew System::Windows::Forms::TextBox());
			this->btnBrowse = (gcnew System::Windows::Forms::Button());
			this->btnSend = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// txbPath
			// 
			this->txbPath->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->txbPath->Location = System::Drawing::Point(12, 12);
			this->txbPath->Name = L"txbPath";
			this->txbPath->Size = System::Drawing::Size(377, 22);
			this->txbPath->TabIndex = 0;
			// 
			// btnBrowse
			// 
			this->btnBrowse->Location = System::Drawing::Point(395, 12);
			this->btnBrowse->Name = L"btnBrowse";
			this->btnBrowse->Size = System::Drawing::Size(75, 25);
			this->btnBrowse->TabIndex = 1;
			this->btnBrowse->Text = L"Browse";
			this->btnBrowse->UseVisualStyleBackColor = true;
			this->btnBrowse->Click += gcnew System::EventHandler(this, &MainForm::btnBrowse_Click);
			// 
			// btnSend
			// 
			this->btnSend->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10.2F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->btnSend->Location = System::Drawing::Point(199, 183);
			this->btnSend->Name = L"btnSend";
			this->btnSend->Size = System::Drawing::Size(75, 30);
			this->btnSend->TabIndex = 2;
			this->btnSend->Text = L"SEND";
			this->btnSend->UseVisualStyleBackColor = true;
			this->btnSend->Click += gcnew System::EventHandler(this, &MainForm::btnSend_Click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(482, 353);
			this->Controls->Add(this->btnSend);
			this->Controls->Add(this->btnBrowse);
			this->Controls->Add(this->txbPath);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MainForm";
			this->Text = L"ProcessA";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		void btnBrowse_Click(System::Object^ sender, System::EventArgs^ e) {
			OpenFileDialog^ openFileDialog = gcnew OpenFileDialog();
			openFileDialog->ShowDialog();
			if (openFileDialog->FileName != "") {
				txbPath->Text = openFileDialog->FileName;
			}
		}
		void btnSend_Click(System::Object^ sender, System::EventArgs^ e) {
			SendFile((LPCWSTR)Marshal::StringToHGlobalUni(txbPath->Text).ToPointer());
		}
	};
}
