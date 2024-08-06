using System.Drawing;
using System.Windows.Forms;

namespace HorizonDemo
{
    partial class MainForm
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.GetVersion_button = new System.Windows.Forms.Button();
            this.SDCard_groupBox = new System.Windows.Forms.GroupBox();
            this.SDCardGetFile_button = new System.Windows.Forms.Button();
            this.SDCardSendFile_button = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.SDCardFileData_textBox = new System.Windows.Forms.TextBox();
            this.SDCardDeleteFile_button = new System.Windows.Forms.Button();
            this.SDCardNewFile_button = new System.Windows.Forms.Button();
            this.SDCardReadFile_button = new System.Windows.Forms.Button();
            this.SDCardWriteFile_button = new System.Windows.Forms.Button();
            this.SDCardCloseFile_button = new System.Windows.Forms.Button();
            this.FileName_textBox = new System.Windows.Forms.TextBox();
            this.SDCardOpenFile_button = new System.Windows.Forms.Button();
            this.CancelFileTransfer_button = new System.Windows.Forms.Button();
            this.Transfer_progressBar = new System.Windows.Forms.ProgressBar();
            this.FirmwareVersion_label = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.FlashGetFile_button = new System.Windows.Forms.Button();
            this.FlashSendFile_button = new System.Windows.Forms.Button();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.FlashReadAddress_textBox = new System.Windows.Forms.TextBox();
            this.FlashWriteReadData_textBox = new System.Windows.Forms.TextBox();
            this.FlashReadLength_textBox = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.FlashWriteAddress_textBox = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.FlashRead_button = new System.Windows.Forms.Button();
            this.FlashWrite_button = new System.Windows.Forms.Button();
            this.FlashEraseBlockCount_textBox = new System.Windows.Forms.TextBox();
            this.FlashEraseAddress_textBox = new System.Windows.Forms.TextBox();
            this.FlashErase_button = new System.Windows.Forms.Button();
            this.Close_button = new System.Windows.Forms.Button();
            this.Logo_pictureBox = new System.Windows.Forms.PictureBox();
            this.USBState_label = new System.Windows.Forms.Label();
            this.FlashGetFileLength_textBox = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.Progress_label = new System.Windows.Forms.Label();
            this.SDCard_groupBox.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Logo_pictureBox)).BeginInit();
            this.SuspendLayout();
            // 
            // GetVersion_button
            // 
            this.GetVersion_button.Location = new System.Drawing.Point(30, 10);
            this.GetVersion_button.Name = "GetVersion_button";
            this.GetVersion_button.Size = new System.Drawing.Size(124, 20);
            this.GetVersion_button.TabIndex = 0;
            this.GetVersion_button.Text = "Get Firmware Version";
            this.GetVersion_button.UseVisualStyleBackColor = true;
            this.GetVersion_button.Click += new System.EventHandler(this.GetVersion_button_Click);
            // 
            // SDCard_groupBox
            // 
            this.SDCard_groupBox.Controls.Add(this.SDCardGetFile_button);
            this.SDCard_groupBox.Controls.Add(this.SDCardSendFile_button);
            this.SDCard_groupBox.Controls.Add(this.label2);
            this.SDCard_groupBox.Controls.Add(this.label1);
            this.SDCard_groupBox.Controls.Add(this.SDCardFileData_textBox);
            this.SDCard_groupBox.Controls.Add(this.SDCardDeleteFile_button);
            this.SDCard_groupBox.Controls.Add(this.SDCardNewFile_button);
            this.SDCard_groupBox.Controls.Add(this.SDCardReadFile_button);
            this.SDCard_groupBox.Controls.Add(this.SDCardWriteFile_button);
            this.SDCard_groupBox.Controls.Add(this.SDCardCloseFile_button);
            this.SDCard_groupBox.Controls.Add(this.FileName_textBox);
            this.SDCard_groupBox.Controls.Add(this.SDCardOpenFile_button);
            this.SDCard_groupBox.Location = new System.Drawing.Point(21, 44);
            this.SDCard_groupBox.Name = "SDCard_groupBox";
            this.SDCard_groupBox.Size = new System.Drawing.Size(392, 221);
            this.SDCard_groupBox.TabIndex = 3;
            this.SDCard_groupBox.TabStop = false;
            this.SDCard_groupBox.Text = "SD Card";
            // 
            // SDCardGetFile_button
            // 
            this.SDCardGetFile_button.Location = new System.Drawing.Point(16, 195);
            this.SDCardGetFile_button.Name = "SDCardGetFile_button";
            this.SDCardGetFile_button.Size = new System.Drawing.Size(72, 20);
            this.SDCardGetFile_button.TabIndex = 14;
            this.SDCardGetFile_button.Text = "Get File...";
            this.SDCardGetFile_button.UseVisualStyleBackColor = true;
            this.SDCardGetFile_button.Click += new System.EventHandler(this.SDCardGetFile_button_Click);
            // 
            // SDCardSendFile_button
            // 
            this.SDCardSendFile_button.Location = new System.Drawing.Point(93, 195);
            this.SDCardSendFile_button.Name = "SDCardSendFile_button";
            this.SDCardSendFile_button.Size = new System.Drawing.Size(74, 20);
            this.SDCardSendFile_button.TabIndex = 13;
            this.SDCardSendFile_button.Text = "Send File...";
            this.SDCardSendFile_button.UseVisualStyleBackColor = true;
            this.SDCardSendFile_button.Click += new System.EventHandler(this.SDCardSendFile_button_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 91);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(65, 13);
            this.label2.TabIndex = 12;
            this.label2.Text = "File content:";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 19);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(55, 13);
            this.label1.TabIndex = 11;
            this.label1.Text = "File name:";
            // 
            // SDCardFileData_textBox
            // 
            this.SDCardFileData_textBox.Location = new System.Drawing.Point(16, 107);
            this.SDCardFileData_textBox.Multiline = true;
            this.SDCardFileData_textBox.Name = "SDCardFileData_textBox";
            this.SDCardFileData_textBox.Size = new System.Drawing.Size(360, 82);
            this.SDCardFileData_textBox.TabIndex = 10;
            // 
            // SDCardDeleteFile_button
            // 
            this.SDCardDeleteFile_button.Location = new System.Drawing.Point(255, 67);
            this.SDCardDeleteFile_button.Name = "SDCardDeleteFile_button";
            this.SDCardDeleteFile_button.Size = new System.Drawing.Size(112, 20);
            this.SDCardDeleteFile_button.TabIndex = 9;
            this.SDCardDeleteFile_button.Text = "Delete File";
            this.SDCardDeleteFile_button.UseVisualStyleBackColor = true;
            this.SDCardDeleteFile_button.Click += new System.EventHandler(this.SDCardDeleteFile_button_Click);
            // 
            // SDCardNewFile_button
            // 
            this.SDCardNewFile_button.Location = new System.Drawing.Point(255, 42);
            this.SDCardNewFile_button.Name = "SDCardNewFile_button";
            this.SDCardNewFile_button.Size = new System.Drawing.Size(112, 20);
            this.SDCardNewFile_button.TabIndex = 8;
            this.SDCardNewFile_button.Text = "New File";
            this.SDCardNewFile_button.UseVisualStyleBackColor = true;
            this.SDCardNewFile_button.Click += new System.EventHandler(this.SDCardNewFile_button_Click);
            // 
            // SDCardReadFile_button
            // 
            this.SDCardReadFile_button.Location = new System.Drawing.Point(138, 41);
            this.SDCardReadFile_button.Name = "SDCardReadFile_button";
            this.SDCardReadFile_button.Size = new System.Drawing.Size(112, 20);
            this.SDCardReadFile_button.TabIndex = 7;
            this.SDCardReadFile_button.Text = "Read File";
            this.SDCardReadFile_button.UseVisualStyleBackColor = true;
            this.SDCardReadFile_button.Click += new System.EventHandler(this.SDCardReadFile_button_Click);
            // 
            // SDCardWriteFile_button
            // 
            this.SDCardWriteFile_button.Location = new System.Drawing.Point(138, 67);
            this.SDCardWriteFile_button.Name = "SDCardWriteFile_button";
            this.SDCardWriteFile_button.Size = new System.Drawing.Size(112, 20);
            this.SDCardWriteFile_button.TabIndex = 6;
            this.SDCardWriteFile_button.Text = "Write File";
            this.SDCardWriteFile_button.UseVisualStyleBackColor = true;
            this.SDCardWriteFile_button.Click += new System.EventHandler(this.SDCardWriteFile_button_Click);
            // 
            // SDCardCloseFile_button
            // 
            this.SDCardCloseFile_button.Location = new System.Drawing.Point(20, 67);
            this.SDCardCloseFile_button.Name = "SDCardCloseFile_button";
            this.SDCardCloseFile_button.Size = new System.Drawing.Size(112, 20);
            this.SDCardCloseFile_button.TabIndex = 5;
            this.SDCardCloseFile_button.Text = "Close File";
            this.SDCardCloseFile_button.UseVisualStyleBackColor = true;
            this.SDCardCloseFile_button.Click += new System.EventHandler(this.SDCardCloseFile_button_Click);
            // 
            // FileName_textBox
            // 
            this.FileName_textBox.Location = new System.Drawing.Point(71, 16);
            this.FileName_textBox.Name = "FileName_textBox";
            this.FileName_textBox.Size = new System.Drawing.Size(305, 20);
            this.FileName_textBox.TabIndex = 4;
            this.FileName_textBox.Text = "horizon.txt";
            // 
            // SDCardOpenFile_button
            // 
            this.SDCardOpenFile_button.Location = new System.Drawing.Point(20, 41);
            this.SDCardOpenFile_button.Name = "SDCardOpenFile_button";
            this.SDCardOpenFile_button.Size = new System.Drawing.Size(112, 20);
            this.SDCardOpenFile_button.TabIndex = 3;
            this.SDCardOpenFile_button.Text = "Open File";
            this.SDCardOpenFile_button.UseVisualStyleBackColor = true;
            this.SDCardOpenFile_button.Click += new System.EventHandler(this.SDCardOpenFile_button_Click);
            // 
            // CancelFileTransfer_button
            // 
            this.CancelFileTransfer_button.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.CancelFileTransfer_button.Location = new System.Drawing.Point(350, 521);
            this.CancelFileTransfer_button.Name = "CancelFileTransfer_button";
            this.CancelFileTransfer_button.Size = new System.Drawing.Size(64, 24);
            this.CancelFileTransfer_button.TabIndex = 16;
            this.CancelFileTransfer_button.Text = "Cancel";
            this.CancelFileTransfer_button.UseVisualStyleBackColor = true;
            this.CancelFileTransfer_button.Click += new System.EventHandler(this.CancelFileTransfer_button_Click);
            // 
            // Transfer_progressBar
            // 
            this.Transfer_progressBar.Location = new System.Drawing.Point(21, 523);
            this.Transfer_progressBar.Name = "Transfer_progressBar";
            this.Transfer_progressBar.Size = new System.Drawing.Size(317, 20);
            this.Transfer_progressBar.TabIndex = 15;
            // 
            // FirmwareVersion_label
            // 
            this.FirmwareVersion_label.AutoSize = true;
            this.FirmwareVersion_label.Font = new System.Drawing.Font("Segoe UI", 9F, System.Drawing.FontStyle.Bold);
            this.FirmwareVersion_label.ForeColor = System.Drawing.Color.Blue;
            this.FirmwareVersion_label.Location = new System.Drawing.Point(160, 13);
            this.FirmwareVersion_label.Name = "FirmwareVersion_label";
            this.FirmwareVersion_label.Size = new System.Drawing.Size(16, 15);
            this.FirmwareVersion_label.TabIndex = 4;
            this.FirmwareVersion_label.Text = "...";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.FlashGetFileLength_textBox);
            this.groupBox1.Controls.Add(this.label9);
            this.groupBox1.Controls.Add(this.FlashGetFile_button);
            this.groupBox1.Controls.Add(this.FlashSendFile_button);
            this.groupBox1.Controls.Add(this.label8);
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.FlashReadAddress_textBox);
            this.groupBox1.Controls.Add(this.FlashWriteReadData_textBox);
            this.groupBox1.Controls.Add(this.FlashReadLength_textBox);
            this.groupBox1.Controls.Add(this.label6);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.FlashWriteAddress_textBox);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.FlashRead_button);
            this.groupBox1.Controls.Add(this.FlashWrite_button);
            this.groupBox1.Controls.Add(this.FlashEraseBlockCount_textBox);
            this.groupBox1.Controls.Add(this.FlashEraseAddress_textBox);
            this.groupBox1.Controls.Add(this.FlashErase_button);
            this.groupBox1.Location = new System.Drawing.Point(21, 271);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(393, 242);
            this.groupBox1.TabIndex = 5;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Flash";
            // 
            // FlashGetFile_button
            // 
            this.FlashGetFile_button.Location = new System.Drawing.Point(16, 216);
            this.FlashGetFile_button.Name = "FlashGetFile_button";
            this.FlashGetFile_button.Size = new System.Drawing.Size(72, 20);
            this.FlashGetFile_button.TabIndex = 20;
            this.FlashGetFile_button.Text = "Get File...";
            this.FlashGetFile_button.UseVisualStyleBackColor = true;
            this.FlashGetFile_button.Click += new System.EventHandler(this.FlashGetFile_button_Click);
            // 
            // FlashSendFile_button
            // 
            this.FlashSendFile_button.Location = new System.Drawing.Point(303, 216);
            this.FlashSendFile_button.Name = "FlashSendFile_button";
            this.FlashSendFile_button.Size = new System.Drawing.Size(74, 20);
            this.FlashSendFile_button.TabIndex = 19;
            this.FlashSendFile_button.Text = "Send File...";
            this.FlashSendFile_button.UseVisualStyleBackColor = true;
            this.FlashSendFile_button.Click += new System.EventHandler(this.FlashSendFile_button_Click);
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(17, 107);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(74, 13);
            this.label8.TabIndex = 18;
            this.label8.Text = "Flash content:";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(104, 77);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(48, 13);
            this.label7.TabIndex = 17;
            this.label7.Text = "Address:";
            // 
            // FlashReadAddress_textBox
            // 
            this.FlashReadAddress_textBox.Location = new System.Drawing.Point(158, 74);
            this.FlashReadAddress_textBox.Name = "FlashReadAddress_textBox";
            this.FlashReadAddress_textBox.Size = new System.Drawing.Size(86, 20);
            this.FlashReadAddress_textBox.TabIndex = 16;
            this.FlashReadAddress_textBox.Text = "0";
            this.FlashReadAddress_textBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Numeric_textBox_KeyPress);
            // 
            // FlashWriteReadData_textBox
            // 
            this.FlashWriteReadData_textBox.Location = new System.Drawing.Point(16, 123);
            this.FlashWriteReadData_textBox.Multiline = true;
            this.FlashWriteReadData_textBox.Name = "FlashWriteReadData_textBox";
            this.FlashWriteReadData_textBox.Size = new System.Drawing.Size(361, 87);
            this.FlashWriteReadData_textBox.TabIndex = 15;
            // 
            // FlashReadLength_textBox
            // 
            this.FlashReadLength_textBox.Location = new System.Drawing.Point(298, 73);
            this.FlashReadLength_textBox.Name = "FlashReadLength_textBox";
            this.FlashReadLength_textBox.Size = new System.Drawing.Size(69, 20);
            this.FlashReadLength_textBox.TabIndex = 14;
            this.FlashReadLength_textBox.Text = "256";
            this.FlashReadLength_textBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Numeric_textBox_KeyPress);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(104, 49);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(48, 13);
            this.label6.TabIndex = 13;
            this.label6.Text = "Address:";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(250, 23);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(67, 13);
            this.label5.TabIndex = 12;
            this.label5.Tag = "";
            this.label5.Text = "Block count:";
            // 
            // FlashWriteAddress_textBox
            // 
            this.FlashWriteAddress_textBox.Location = new System.Drawing.Point(158, 45);
            this.FlashWriteAddress_textBox.Name = "FlashWriteAddress_textBox";
            this.FlashWriteAddress_textBox.Size = new System.Drawing.Size(86, 20);
            this.FlashWriteAddress_textBox.TabIndex = 11;
            this.FlashWriteAddress_textBox.Text = "0";
            this.FlashWriteAddress_textBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Numeric_textBox_KeyPress);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(252, 77);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(40, 13);
            this.label4.TabIndex = 10;
            this.label4.Text = "Length";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(104, 23);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(48, 13);
            this.label3.TabIndex = 9;
            this.label3.Text = "Address:";
            // 
            // FlashRead_button
            // 
            this.FlashRead_button.Location = new System.Drawing.Point(16, 73);
            this.FlashRead_button.Name = "FlashRead_button";
            this.FlashRead_button.Size = new System.Drawing.Size(82, 20);
            this.FlashRead_button.TabIndex = 8;
            this.FlashRead_button.Text = "Read";
            this.FlashRead_button.UseVisualStyleBackColor = true;
            this.FlashRead_button.Click += new System.EventHandler(this.FlashRead_button_Click);
            // 
            // FlashWrite_button
            // 
            this.FlashWrite_button.Location = new System.Drawing.Point(16, 45);
            this.FlashWrite_button.Name = "FlashWrite_button";
            this.FlashWrite_button.Size = new System.Drawing.Size(82, 20);
            this.FlashWrite_button.TabIndex = 7;
            this.FlashWrite_button.Text = "Write";
            this.FlashWrite_button.UseVisualStyleBackColor = true;
            this.FlashWrite_button.Click += new System.EventHandler(this.FlashWrite_button_Click);
            // 
            // FlashEraseBlockCount_textBox
            // 
            this.FlashEraseBlockCount_textBox.Location = new System.Drawing.Point(314, 20);
            this.FlashEraseBlockCount_textBox.Name = "FlashEraseBlockCount_textBox";
            this.FlashEraseBlockCount_textBox.Size = new System.Drawing.Size(53, 20);
            this.FlashEraseBlockCount_textBox.TabIndex = 6;
            this.FlashEraseBlockCount_textBox.Text = "1";
            this.FlashEraseBlockCount_textBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Numeric_textBox_KeyPress);
            // 
            // FlashEraseAddress_textBox
            // 
            this.FlashEraseAddress_textBox.Location = new System.Drawing.Point(158, 19);
            this.FlashEraseAddress_textBox.Name = "FlashEraseAddress_textBox";
            this.FlashEraseAddress_textBox.Size = new System.Drawing.Size(86, 20);
            this.FlashEraseAddress_textBox.TabIndex = 5;
            this.FlashEraseAddress_textBox.Text = "0";
            this.FlashEraseAddress_textBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Numeric_textBox_KeyPress);
            // 
            // FlashErase_button
            // 
            this.FlashErase_button.Location = new System.Drawing.Point(16, 19);
            this.FlashErase_button.Name = "FlashErase_button";
            this.FlashErase_button.Size = new System.Drawing.Size(82, 20);
            this.FlashErase_button.TabIndex = 4;
            this.FlashErase_button.Text = "Erase";
            this.FlashErase_button.UseVisualStyleBackColor = true;
            this.FlashErase_button.Click += new System.EventHandler(this.FlashErase_button_Click);
            // 
            // Close_button
            // 
            this.Close_button.Location = new System.Drawing.Point(179, 553);
            this.Close_button.Name = "Close_button";
            this.Close_button.Size = new System.Drawing.Size(75, 30);
            this.Close_button.TabIndex = 6;
            this.Close_button.Text = "Close";
            this.Close_button.UseVisualStyleBackColor = true;
            this.Close_button.Click += new System.EventHandler(this.Close_button_Click);
            // 
            // Logo_pictureBox
            // 
            this.Logo_pictureBox.Image = global::HorizonDemo.Properties.Resources.Singular_01;
            this.Logo_pictureBox.Location = new System.Drawing.Point(216, 3);
            this.Logo_pictureBox.Name = "Logo_pictureBox";
            this.Logo_pictureBox.Size = new System.Drawing.Size(195, 46);
            this.Logo_pictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.Logo_pictureBox.TabIndex = 7;
            this.Logo_pictureBox.TabStop = false;
            // 
            // USBState_label
            // 
            this.USBState_label.AutoSize = true;
            this.USBState_label.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.USBState_label.ForeColor = System.Drawing.Color.Red;
            this.USBState_label.Location = new System.Drawing.Point(18, 560);
            this.USBState_label.Name = "USBState_label";
            this.USBState_label.Size = new System.Drawing.Size(20, 17);
            this.USBState_label.TabIndex = 8;
            this.USBState_label.Text = "...";
            // 
            // FlashGetFileLength_textBox
            // 
            this.FlashGetFileLength_textBox.Location = new System.Drawing.Point(164, 217);
            this.FlashGetFileLength_textBox.Name = "FlashGetFileLength_textBox";
            this.FlashGetFileLength_textBox.Size = new System.Drawing.Size(69, 20);
            this.FlashGetFileLength_textBox.TabIndex = 22;
            this.FlashGetFileLength_textBox.Text = "256";
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(94, 220);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(75, 13);
            this.label9.TabIndex = 21;
            this.label9.Text = "Get file length:";
            // 
            // Progress_label
            // 
            this.Progress_label.AutoSize = true;
            this.Progress_label.Location = new System.Drawing.Point(24, 546);
            this.Progress_label.Name = "Progress_label";
            this.Progress_label.Size = new System.Drawing.Size(16, 13);
            this.Progress_label.TabIndex = 22;
            this.Progress_label.Text = "...";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(436, 595);
            this.ControlBox = false;
            this.Controls.Add(this.Progress_label);
            this.Controls.Add(this.CancelFileTransfer_button);
            this.Controls.Add(this.USBState_label);
            this.Controls.Add(this.Transfer_progressBar);
            this.Controls.Add(this.Logo_pictureBox);
            this.Controls.Add(this.Close_button);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.FirmwareVersion_label);
            this.Controls.Add(this.SDCard_groupBox);
            this.Controls.Add(this.GetVersion_button);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Horizon PIC32MZ Demo";
            this.SDCard_groupBox.ResumeLayout(false);
            this.SDCard_groupBox.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.Logo_pictureBox)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private Button GetVersion_button;
        private GroupBox SDCard_groupBox;
        private TextBox FileName_textBox;
        private Button SDCardOpenFile_button;
        private Button SDCardDeleteFile_button;
        private Button SDCardNewFile_button;
        private Button SDCardReadFile_button;
        private Button SDCardWriteFile_button;
        private Button SDCardCloseFile_button;
        private TextBox SDCardFileData_textBox;
        private Label FirmwareVersion_label;
        private Label label2;
        private Label label1;
        private GroupBox groupBox1;
        private Button FlashErase_button;
        private TextBox FlashEraseBlockCount_textBox;
        private TextBox FlashEraseAddress_textBox;
        private Button FlashRead_button;
        private Button FlashWrite_button;
        private TextBox FlashReadLength_textBox;
        private Label label6;
        private Label label5;
        private TextBox FlashWriteAddress_textBox;
        private Label label4;
        private Label label3;
        private TextBox FlashWriteReadData_textBox;
        private Button Close_button;
        private Label label8;
        private Label label7;
        private TextBox FlashReadAddress_textBox;
        private Button SDCardGetFile_button;
        private Button SDCardSendFile_button;
        private ProgressBar Transfer_progressBar;
        private PictureBox Logo_pictureBox;
        private Label USBState_label;
        private Button CancelFileTransfer_button;
        private Button FlashGetFile_button;
        private Button FlashSendFile_button;
        private TextBox FlashGetFileLength_textBox;
        private Label label9;
        private Label Progress_label;
    }
}