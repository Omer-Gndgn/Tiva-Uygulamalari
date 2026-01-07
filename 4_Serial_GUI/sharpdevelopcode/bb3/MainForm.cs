using System;
using System.Windows.Forms;
using System.IO.Ports;

namespace bb3
{
    public partial class MainForm : Form
    {
        private string rxBuffer = "";

        public MainForm()
        {
            InitializeComponent();

            serialPort1.BaudRate = 115200;
            serialPort1.DataBits = 8;
            serialPort1.Parity   = Parity.None;
            serialPort1.StopBits = StopBits.One;

            serialPort1.DataReceived += SerialPort1_DataReceived;
        }

        // ===============================
        // PORT AÇ
        // ===============================
        void ButtonPORTACClick(object sender, EventArgs e)
        {
            try
            {
                if (serialPort1.IsOpen) return;
                serialPort1.PortName = textBoxPORTNO.Text.Trim();
                serialPort1.Open();
            }
            catch (Exception ex)
            {
                MessageBox.Show("Port açılamadı: " + ex.Message);
            }
        }

        // ===============================
        // PORT KAPAT
        // ===============================
        void ButtonportkapatClick(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
                serialPort1.Close();
        }

        // ===============================
        // METİN GÖNDER (t2 -> LCD üst satır 3 karakter)
        // Protokol: @ABC
        // ===============================
        
        
        
        void metingonderClick(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
            {
                MessageBox.Show("Önce portu aç");
                return;
            }

            // t2 içine ABC yaz
            string s = textBox1.Text.Trim();
            if (s.Length < 3) s = s.PadRight(3, ' ');
            if (s.Length > 3) s = s.Substring(0, 3);

         serialPort1.Write("<" + s + ">");
   // \r\n YOK
        }

        // ===============================
        // SAAT GÖNDER (t1 -> Tiva saati ayarla)
        // Protokol: !12:34:56\r\n
        // ===============================
        void saatgonderClick(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
            {
                MessageBox.Show("Önce portu aç");
                return;
            }

            // t1 içine 12:34:56 yaz
            string s = t1.Text.Trim();
            serialPort1.WriteLine("!" + s); // satır sonu OK
        }

        // ===============================
        // MCU'DAN VERİ GELDİ  (*ADC , #SAAT , B1)
        // ===============================
        void SerialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                string okunan = serialPort1.ReadExisting();

                BeginInvoke(new MethodInvoker(delegate
                {
                    rxBuffer += okunan;
                    rxBuffer = rxBuffer.Replace('\r', '\n');

                    int idx;
                    while ((idx = rxBuffer.IndexOf('\n')) >= 0)
                    {
                        string satir = rxBuffer.Substring(0, idx).Trim();
                        rxBuffer = rxBuffer.Substring(idx + 1);

                        if (satir.Length == 0) continue;

                        if (satir[0] == '*')          // ADC: *123
                            t3.Text = satir.Substring(1);
                        else if (satir[0] == '#')     // SAAT: #12:34:56
                            t5.Text = satir.Substring(1);
                        else if (satir == "B1")       // BUTON
                            butongelen.Text = "Buton 1'e basıldı";
                    }
                }));
            }
            catch { }
        }

        // Designer boş handler’lar (gerekirse)
        void TextBox1TextChanged(object sender, EventArgs e) { }
        void Label3Click(object sender, EventArgs e) { }
        void Button1Click(object sender, EventArgs e) { }
        void GelenveriTextChanged(object sender, EventArgs e) { }
        void Label5Click(object sender, EventArgs e) { }
        // === Designer.cs bunu istiyor olabilir (saatgonder butonu buna bağlı) ===
void uttonmetinGondernClick(object sender, EventArgs e)
{
    // İstersen burada saatgonderClick'i çağır:
    saatgonderClick(sender, e);
}

// === Designer.cs metin gönder butonunu buna bağlıyorsa ===
void ButtometinGondernClick(object sender, EventArgs e)
{
    // İstersen burada metingonderClick'i çağır:
    metingonderClick(sender, e);
}

		
		void MetingonderClick(object sender, EventArgs e)
		{
			metingonderClick(sender, e);
		}
		
		void Button2Click(object sender, EventArgs e)
		{

{
    // Bu satır senin yazdığın asıl fonksiyonu çağırır
    metingonderClick(sender, e);
}			
		}
    }
}
