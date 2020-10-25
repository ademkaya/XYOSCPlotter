namespace ita
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
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
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.GroupBox groupBox1;
            this.btnAminating = new System.Windows.Forms.Button();
            this.lstFiles = new System.Windows.Forms.ListBox();
            this.btnAddAnimation = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.chkBoxSwapXY = new System.Windows.Forms.CheckBox();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.pBox1 = new System.Windows.Forms.PictureBox();
            this.chkContinuousSend = new System.Windows.Forms.CheckBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.chkClock = new System.Windows.Forms.CheckBox();
            this.btnSingleSend = new System.Windows.Forms.Button();
            this.btnProcess = new System.Windows.Forms.Button();
            this.chkRevY = new System.Windows.Forms.CheckBox();
            this.chkRevX = new System.Windows.Forms.CheckBox();
            this.btnload = new System.Windows.Forms.Button();
            this.pBox = new System.Windows.Forms.PictureBox();
            this.tmr = new System.Windows.Forms.Timer(this.components);
            this.serPort = new System.IO.Ports.SerialPort(this.components);
            this.tmrClock = new System.Windows.Forms.Timer(this.components);
            groupBox1 = new System.Windows.Forms.GroupBox();
            groupBox1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.groupBox3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pBox1)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pBox)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            groupBox1.Controls.Add(this.btnAminating);
            groupBox1.Controls.Add(this.lstFiles);
            groupBox1.Controls.Add(this.btnAddAnimation);
            groupBox1.ImeMode = System.Windows.Forms.ImeMode.NoControl;
            groupBox1.Location = new System.Drawing.Point(424, 12);
            groupBox1.Margin = new System.Windows.Forms.Padding(4);
            groupBox1.Name = "groupBox1";
            groupBox1.Padding = new System.Windows.Forms.Padding(4);
            groupBox1.Size = new System.Drawing.Size(333, 271);
            groupBox1.TabIndex = 11;
            groupBox1.TabStop = false;
            groupBox1.Text = "Animation";
            // 
            // btnAminating
            // 
            this.btnAminating.Location = new System.Drawing.Point(8, 208);
            this.btnAminating.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnAminating.Name = "btnAminating";
            this.btnAminating.Size = new System.Drawing.Size(309, 42);
            this.btnAminating.TabIndex = 15;
            this.btnAminating.Text = "Start animating";
            this.btnAminating.UseVisualStyleBackColor = true;
            this.btnAminating.Click += new System.EventHandler(this.btnAminating_Click);
            // 
            // lstFiles
            // 
            this.lstFiles.FormattingEnabled = true;
            this.lstFiles.ItemHeight = 16;
            this.lstFiles.Location = new System.Drawing.Point(8, 70);
            this.lstFiles.Margin = new System.Windows.Forms.Padding(4);
            this.lstFiles.Name = "lstFiles";
            this.lstFiles.Size = new System.Drawing.Size(309, 132);
            this.lstFiles.TabIndex = 13;
            // 
            // btnAddAnimation
            // 
            this.btnAddAnimation.Location = new System.Drawing.Point(8, 22);
            this.btnAddAnimation.Margin = new System.Windows.Forms.Padding(4);
            this.btnAddAnimation.Name = "btnAddAnimation";
            this.btnAddAnimation.Size = new System.Drawing.Size(309, 41);
            this.btnAddAnimation.TabIndex = 14;
            this.btnAddAnimation.Text = "Add animation folder";
            this.btnAddAnimation.UseVisualStyleBackColor = true;
            this.btnAddAnimation.Click += new System.EventHandler(this.btnAddAnimation_Click);
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.chkBoxSwapXY);
            this.panel1.Controls.Add(this.groupBox3);
            this.panel1.Controls.Add(this.groupBox2);
            this.panel1.Controls.Add(groupBox1);
            this.panel1.Controls.Add(this.btnSingleSend);
            this.panel1.Controls.Add(this.btnProcess);
            this.panel1.Controls.Add(this.chkRevY);
            this.panel1.Controls.Add(this.chkRevX);
            this.panel1.Controls.Add(this.btnload);
            this.panel1.Controls.Add(this.pBox);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(1176, 473);
            this.panel1.TabIndex = 0;
            // 
            // chkBoxSwapXY
            // 
            this.chkBoxSwapXY.AutoSize = true;
            this.chkBoxSwapXY.Location = new System.Drawing.Point(318, 66);
            this.chkBoxSwapXY.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.chkBoxSwapXY.Name = "chkBoxSwapXY";
            this.chkBoxSwapXY.Size = new System.Drawing.Size(86, 21);
            this.chkBoxSwapXY.TabIndex = 14;
            this.chkBoxSwapXY.Text = "Swap XY";
            this.chkBoxSwapXY.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.pBox1);
            this.groupBox3.Controls.Add(this.chkContinuousSend);
            this.groupBox3.Location = new System.Drawing.Point(768, 18);
            this.groupBox3.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox3.Size = new System.Drawing.Size(388, 450);
            this.groupBox3.TabIndex = 13;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Continuos";
            // 
            // pBox1
            // 
            this.pBox1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pBox1.Location = new System.Drawing.Point(19, 22);
            this.pBox1.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.pBox1.Name = "pBox1";
            this.pBox1.Size = new System.Drawing.Size(349, 349);
            this.pBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pBox1.TabIndex = 7;
            this.pBox1.TabStop = false;
            // 
            // chkContinuousSend
            // 
            this.chkContinuousSend.AutoSize = true;
            this.chkContinuousSend.Location = new System.Drawing.Point(19, 393);
            this.chkContinuousSend.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.chkContinuousSend.Name = "chkContinuousSend";
            this.chkContinuousSend.Size = new System.Drawing.Size(138, 21);
            this.chkContinuousSend.TabIndex = 11;
            this.chkContinuousSend.Text = "Continuous Send";
            this.chkContinuousSend.UseVisualStyleBackColor = true;
            this.chkContinuousSend.CheckedChanged += new System.EventHandler(this.chkContinuousSend_CheckedChanged);
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.chkClock);
            this.groupBox2.Location = new System.Drawing.Point(424, 366);
            this.groupBox2.Margin = new System.Windows.Forms.Padding(4);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Padding = new System.Windows.Forms.Padding(4);
            this.groupBox2.Size = new System.Drawing.Size(333, 66);
            this.groupBox2.TabIndex = 12;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Clock";
            // 
            // chkClock
            // 
            this.chkClock.AutoSize = true;
            this.chkClock.Location = new System.Drawing.Point(8, 22);
            this.chkClock.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.chkClock.Name = "chkClock";
            this.chkClock.Size = new System.Drawing.Size(166, 21);
            this.chkClock.TabIndex = 16;
            this.chkClock.Text = "Activate Analog Clock";
            this.chkClock.UseVisualStyleBackColor = true;
            this.chkClock.CheckedChanged += new System.EventHandler(this.chkClock_CheckedChanged);
            // 
            // btnSingleSend
            // 
            this.btnSingleSend.Location = new System.Drawing.Point(304, 427);
            this.btnSingleSend.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnSingleSend.Name = "btnSingleSend";
            this.btnSingleSend.Size = new System.Drawing.Size(92, 42);
            this.btnSingleSend.TabIndex = 10;
            this.btnSingleSend.Text = "Send";
            this.btnSingleSend.UseVisualStyleBackColor = true;
            this.btnSingleSend.Click += new System.EventHandler(this.btnSingleSend_Click);
            // 
            // btnProcess
            // 
            this.btnProcess.Location = new System.Drawing.Point(12, 427);
            this.btnProcess.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnProcess.Name = "btnProcess";
            this.btnProcess.Size = new System.Drawing.Size(285, 42);
            this.btnProcess.TabIndex = 6;
            this.btnProcess.Text = "Process";
            this.btnProcess.UseVisualStyleBackColor = true;
            this.btnProcess.Click += new System.EventHandler(this.btnProcess_Click);
            // 
            // chkRevY
            // 
            this.chkRevY.AutoSize = true;
            this.chkRevY.Checked = true;
            this.chkRevY.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkRevY.Location = new System.Drawing.Point(103, 65);
            this.chkRevY.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.chkRevY.Name = "chkRevY";
            this.chkRevY.Size = new System.Drawing.Size(85, 21);
            this.chkRevY.TabIndex = 3;
            this.chkRevY.Text = "Revert Y";
            this.chkRevY.UseVisualStyleBackColor = true;
            // 
            // chkRevX
            // 
            this.chkRevX.AutoSize = true;
            this.chkRevX.Location = new System.Drawing.Point(12, 65);
            this.chkRevX.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.chkRevX.Name = "chkRevX";
            this.chkRevX.Size = new System.Drawing.Size(85, 21);
            this.chkRevX.TabIndex = 2;
            this.chkRevX.Text = "Revert X";
            this.chkRevX.UseVisualStyleBackColor = true;
            // 
            // btnload
            // 
            this.btnload.Location = new System.Drawing.Point(12, 12);
            this.btnload.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.btnload.Name = "btnload";
            this.btnload.Size = new System.Drawing.Size(392, 42);
            this.btnload.TabIndex = 1;
            this.btnload.Text = "Select Image";
            this.btnload.UseVisualStyleBackColor = true;
            this.btnload.Click += new System.EventHandler(this.btn_load_Click);
            // 
            // pBox
            // 
            this.pBox.Location = new System.Drawing.Point(0, 0);
            this.pBox.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.pBox.Name = "pBox";
            this.pBox.Size = new System.Drawing.Size(417, 470);
            this.pBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pBox.TabIndex = 0;
            this.pBox.TabStop = false;
            // 
            // tmr
            // 
            this.tmr.Interval = 250;
            this.tmr.Tick += new System.EventHandler(this.tmr_Tick);
            // 
            // serPort
            // 
            this.serPort.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serPort_DataReceived);
            // 
            // tmrClock
            // 
            this.tmrClock.Tick += new System.EventHandler(this.tmrClock_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1176, 473);
            this.Controls.Add(this.panel1);
            this.Margin = new System.Windows.Forms.Padding(3, 2, 3, 2);
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            groupBox1.ResumeLayout(false);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pBox1)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pBox)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.PictureBox pBox;
        private System.Windows.Forms.Button btnload;
        private System.Windows.Forms.Button btnProcess;
        private System.Windows.Forms.CheckBox chkRevY;
        private System.Windows.Forms.CheckBox chkRevX;
        private System.Windows.Forms.PictureBox pBox1;
        private System.Windows.Forms.Timer tmr;
        private System.IO.Ports.SerialPort serPort;
        private System.Windows.Forms.Button btnSingleSend;
        private System.Windows.Forms.CheckBox chkContinuousSend;
        private System.Windows.Forms.Button btnAddAnimation;
        private System.Windows.Forms.ListBox lstFiles;
        private System.Windows.Forms.Button btnAminating;
        private System.Windows.Forms.Timer tmrClock;
        private System.Windows.Forms.CheckBox chkClock;
        private System.Windows.Forms.GroupBox groupBox3;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.CheckBox chkBoxSwapXY;
    }
}

