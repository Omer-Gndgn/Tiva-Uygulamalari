/*
 * Created by SharpDevelop.
 * User: ardayavuz
 * Date: 4.12.2025
 * Time: 14:53
 * 
 * To change this template use Tools | Options | Coding | Edit Standard Headers.
 */
namespace bb3
{
	partial class MainForm
	{
		/// <summary>
		/// Designer variable used to keep track of non-visual components.
		/// </summary>
		private System.ComponentModel.IContainer components = null;
		
		/// <summary>
		/// Disposes resources used by the form.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing) {
				if (components != null) {
					components.Dispose();
				}
			}
			base.Dispose(disposing);
		}
		
		/// <summary>
		/// This method is required for Windows Forms designer support.
		/// Do not change the method contents inside the source code editor. The Forms designer might
		/// not be able to load this method if it was changed manually.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
			this.label1 = new System.Windows.Forms.Label();
			this.textBoxPORTNO = new System.Windows.Forms.TextBox();
			this.buttonPORTAC = new System.Windows.Forms.Button();
			this.buttonportkapat = new System.Windows.Forms.Button();
			this.label2 = new System.Windows.Forms.Label();
			this.t1 = new System.Windows.Forms.TextBox();
			this.saatgonder = new System.Windows.Forms.Button();
			this.label4 = new System.Windows.Forms.Label();
			this.t3 = new System.Windows.Forms.TextBox();
			this.label5 = new System.Windows.Forms.Label();
			this.label6 = new System.Windows.Forms.Label();
			this.t5 = new System.Windows.Forms.TextBox();
			this.butongelen = new System.Windows.Forms.TextBox();
			this.button1 = new System.Windows.Forms.Button();
			this.textBox1 = new System.Windows.Forms.TextBox();
			this.button2 = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(19, 30);
			this.label1.Margin = new System.Windows.Forms.Padding(10, 0, 10, 0);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(117, 40);
			this.label1.TabIndex = 0;
			this.label1.Text = "portno";
			// 
			// textBoxPORTNO
			// 
			this.textBoxPORTNO.Location = new System.Drawing.Point(165, 30);
			this.textBoxPORTNO.Name = "textBoxPORTNO";
			this.textBoxPORTNO.Size = new System.Drawing.Size(130, 53);
			this.textBoxPORTNO.TabIndex = 1;
			// 
			// buttonPORTAC
			// 
			this.buttonPORTAC.Location = new System.Drawing.Point(340, 30);
			this.buttonPORTAC.Name = "buttonPORTAC";
			this.buttonPORTAC.Size = new System.Drawing.Size(176, 52);
			this.buttonPORTAC.TabIndex = 2;
			this.buttonPORTAC.Text = "port aç";
			this.buttonPORTAC.UseVisualStyleBackColor = true;
			this.buttonPORTAC.Click += new System.EventHandler(this.ButtonPORTACClick);
			// 
			// buttonportkapat
			// 
			this.buttonportkapat.Location = new System.Drawing.Point(574, 30);
			this.buttonportkapat.Name = "buttonportkapat";
			this.buttonportkapat.Size = new System.Drawing.Size(168, 52);
			this.buttonportkapat.TabIndex = 3;
			this.buttonportkapat.Text = "KAPAT";
			this.buttonportkapat.UseVisualStyleBackColor = true;
			this.buttonportkapat.Click += new System.EventHandler(this.ButtonportkapatClick);
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(19, 115);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(299, 43);
			this.label2.TabIndex = 4;
			this.label2.Text = "saat gonder";
			// 
			// t1
			// 
			this.t1.Location = new System.Drawing.Point(339, 118);
			this.t1.Name = "t1";
			this.t1.Size = new System.Drawing.Size(286, 53);
			this.t1.TabIndex = 5;
			// 
			// saatgonder
			// 
			this.saatgonder.Location = new System.Drawing.Point(654, 115);
			this.saatgonder.Name = "saatgonder";
			this.saatgonder.Size = new System.Drawing.Size(175, 51);
			this.saatgonder.TabIndex = 6;
			this.saatgonder.Text = "Gönder";
			this.saatgonder.UseVisualStyleBackColor = true;
			this.saatgonder.Click += new System.EventHandler(this.uttonmetinGondernClick);
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(19, 252);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(206, 68);
			this.label4.TabIndex = 9;
			this.label4.Text = "gelen saat";
			// 
			// t3
			// 
			this.t3.Location = new System.Drawing.Point(335, 319);
			this.t3.Name = "t3";
			this.t3.Size = new System.Drawing.Size(217, 53);
			this.t3.TabIndex = 10;
			this.t3.TextChanged += new System.EventHandler(this.GelenveriTextChanged);
			// 
			// label5
			// 
			this.label5.Location = new System.Drawing.Point(30, 316);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(241, 73);
			this.label5.TabIndex = 13;
			this.label5.Text = "adc verisi";
			this.label5.Click += new System.EventHandler(this.Label5Click);
			// 
			// label6
			// 
			this.label6.Location = new System.Drawing.Point(42, 404);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(265, 42);
			this.label6.TabIndex = 15;
			this.label6.Text = "buton durumu";
			// 
			// t5
			// 
			this.t5.Location = new System.Drawing.Point(277, 260);
			this.t5.Name = "t5";
			this.t5.Size = new System.Drawing.Size(363, 53);
			this.t5.TabIndex = 16;
			// 
			// butongelen
			// 
			this.butongelen.Location = new System.Drawing.Point(340, 420);
			this.butongelen.Name = "butongelen";
			this.butongelen.Size = new System.Drawing.Size(658, 53);
			this.butongelen.TabIndex = 17;
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(713, 313);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(175, 59);
			this.button1.TabIndex = 14;
			this.button1.Text = "button1";
			this.button1.UseVisualStyleBackColor = true;
			this.button1.Click += new System.EventHandler(this.Button1Click);
			// 
			// textBox1
			// 
			this.textBox1.Location = new System.Drawing.Point(274, 191);
			this.textBox1.Name = "textBox1";
			this.textBox1.Size = new System.Drawing.Size(393, 53);
			this.textBox1.TabIndex = 18;
			// 
			// button2
			// 
			this.button2.Location = new System.Drawing.Point(714, 189);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(204, 63);
			this.button2.TabIndex = 19;
			this.button2.Text = "button2";
			this.button2.UseVisualStyleBackColor = true;
			this.button2.Click += new System.EventHandler(this.Button2Click);
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(23F, 46F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(1047, 641);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.textBox1);
			this.Controls.Add(this.butongelen);
			this.Controls.Add(this.t5);
			this.Controls.Add(this.label6);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.t3);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.saatgonder);
			this.Controls.Add(this.t1);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.buttonportkapat);
			this.Controls.Add(this.buttonPORTAC);
			this.Controls.Add(this.textBoxPORTNO);
			this.Controls.Add(this.label1);
			this.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
			this.Margin = new System.Windows.Forms.Padding(10, 9, 10, 9);
			this.Name = "MainForm";
			this.Text = "bb3";
			this.ResumeLayout(false);
			this.PerformLayout();
		}
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.TextBox textBox1;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.TextBox butongelen;
		private System.Windows.Forms.TextBox t5;
		private System.Windows.Forms.Label label6;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.TextBox t3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Button saatgonder;
		private System.Windows.Forms.TextBox t1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Button buttonportkapat;
		private System.Windows.Forms.Button buttonPORTAC;
		private System.Windows.Forms.TextBox textBoxPORTNO;
		private System.Windows.Forms.Label label1;
		private System.IO.Ports.SerialPort serialPort1;
	}
}
