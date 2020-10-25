using ita.Properties;
using Microsoft.Win32;
using System;
using System.CodeDom.Compiler;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;
using System.Drawing.Text;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Resources;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ita
{
    public partial class Form1 : Form
    {
        public readonly string[] ImageTypes = new string[] { ".bmp", ".jpg", ".png", ".gif", ".jpeg", ".tiff" };

        private readonly string MapText = "0;255";

        public readonly string DAClimits         = "DAClimits";
        public readonly string OUTFolder         = "OUTFolder";
        public readonly string AnimFolder        = "AnimFolder";

        public readonly string transferCompleted = "Txx";
        public readonly string handshakeReceive  = "Mxx";
        public readonly string handshakeSend     = "M??";
        public readonly string StateSend         = "Sxx";

        private readonly SendArgs dataSendArgs = new SendArgs(false);
        private readonly SendArgs animSendArgs = new SendArgs(false);        
        private readonly IsProcessDone ipd = new IsProcessDone(false, string.Empty);
        private clockArgs cArgs;

        public Thread DataSend;
        public Thread ConnectionCheck;
        public Thread AnimationSend;
        public Thread ProcessClock;

        public RegistryKey key;
        public string selectedFile = string.Empty;
        
        public List<Point> blackPixels;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            key = Registry.CurrentUser.OpenSubKey(@"SOFTWARE\YTImager", true);
            if (key == null)
            {
                key = Registry.CurrentUser.CreateSubKey(@"SOFTWARE\YTImager", true);
            }
                
                serPort.BaudRate = 3000000;
                serPort.DataBits = 8;
                serPort.StopBits = System.IO.Ports.StopBits.One;
                serPort.Parity = System.IO.Ports.Parity.None;
                serPort.PortName = "COM20";
                //serPort.Open();            

            if (!serPort.IsOpen)
            {
                // start querying
                ConnectionCheck = new Thread(() => fConnectionCheck(ipd));
                ConnectionCheck.Start();
            }
            DataSend        = new Thread(()=> fDataSend(ref blackPixels, dataSendArgs));
            AnimationSend   = new Thread(()=> fAnimationSend(ref blackPixels, animSendArgs));

            Bitmap    clock = ita.Properties.Resources.clock;
            cArgs        = new clockArgs(false, false, ref clock);
            ProcessClock = new Thread(() => fprocessClock(cArgs));
            ProcessClock.Start();

            tmrClock.Tag = cArgs;
            tmrClock.Interval = 500;
            tmrClock.Enabled = true;
            tmrClock.Stop();


            tmr.Interval = 300;
            tmr.Enabled = false;

        }

        #region THREAD Functions ------------------------------------------------------------------------------
            private void fDataSend(ref List<Point> _blackPixels,SendArgs arguments)
            {
                while ((!arguments.IsDone) && (!arguments.Quitting))
                {
                    sendFrameSizeData(_blackPixels);
                    List<Point> castedList = ImageCasting(_blackPixels, MapText, chkRevX.Checked, chkRevY.Checked,chkBoxSwapXY.Checked);
                    byte[] temp = new byte[3] { 0, 0, 0};

                    StringBuilder line = new StringBuilder();
                    foreach (Point p in castedList)
                    {
                        temp[0] = (byte)'D';
                        temp[1] = (byte)p.X;
                        temp[2] = (byte)p.Y;

                        if (serPort.IsOpen)
                            serPort.Write(temp, 0, temp.Length);
                    }

                    arguments.IsDone = true;
                    while((arguments.IsDone) && (!arguments.Quitting)){
                        Thread.Sleep(1);
                    }

                }
                DataSend.Abort();
            }
            private void fConnectionCheck(IsProcessDone ipd)                       
            {
                string[] portNames = SerialPort.GetPortNames();
                this.Invoke((MethodInvoker)(() => this.Text = "Searching for connection ... "));
                int cntr = 0;

                while ((!ipd.IsDone) && (!ipd.Quitting))
                {
                    Thread.Sleep(10);

                    if (ipd.IsDone)
                        break;

                    serPort.Close();
                    ipd.SetPortName(portNames[cntr]);
                    serPort.PortName = ipd.getPortName;

                    if (!serPort.IsOpen)
                    {
                        try
                        {
                            serPort.Open();
                        }
                        catch
                        {
                            cntr += 1;

                            if (cntr == portNames.Length)
                            {
                                cntr = 0;
                                portNames = SerialPort.GetPortNames();
                            }
                            continue;
                        }
                    }

                    serPort.Write(handshakeSend);

                    cntr += 1;
                    if (cntr == portNames.Length)
                    {
                        cntr = 0;
                        portNames = SerialPort.GetPortNames();
                    }

                    Thread.Sleep(10);

                }

                ConnectionCheck.Abort();
            }
            private void fAnimationSend(ref List<Point> _blackPixels, SendArgs arguments)
            {
                byte lateACKcntr = 0;
                int prv = 0;
                int fileCntr = 0;

                int listCount = 0;
                string fileName = string.Empty;
                string path = string.Empty;

                List<string> files = new List<string>();
                arguments.Update = true;

                while ((!arguments.IsDone) && (!arguments.Quitting))
                {

                    if (arguments.Update)
                    {
                        arguments.Update = false;
                        fileCntr = 0;

                        path = key.GetValue(AnimFolder).ToString();

                        if (files.Count != 0)
                            files.Clear();

                        this.Invoke((MethodInvoker)(() => listCount = lstFiles.Items.Count));
                        for (int i = 0; i < listCount; i++)
                        {
                            this.Invoke((MethodInvoker)(() => fileName = lstFiles.Items[i].ToString()));
                            files.Add(path + "\\" + fileName);
                        }
                    }

                        Bitmap img = (Bitmap)Bitmap.FromFile(files[fileCntr]);
                        AddBorder(ref img);
                        _blackPixels = getBlackPixels(img);
                        img.Dispose();

                        if (prv != _blackPixels.Count)
                        {
                            sendFrameSizeData(_blackPixels);
                            prv = _blackPixels.Count;
                            Thread.Sleep(1);
                        }
                        fileCntr += 1;
                        if (fileCntr == files.Count)
                            fileCntr = 0;

                        List<Point> castedList = ImageCasting(_blackPixels, MapText, chkRevX.Checked, chkRevY.Checked, chkBoxSwapXY.Checked);
                        byte[] temp = new byte[3] { 0, 0, 0 };
                        StringBuilder line = new StringBuilder();
                        foreach (Point p in castedList)
                        {
                            temp[0] = (byte)'D';
                            temp[1] = (byte)p.X;
                            temp[2] = (byte)p.Y;
                            serPort.Write(temp, 0, temp.Length);
                        }
                    
                    /*if ack not received ask again !!!*/
                    while (((arguments.IsDone) || (!arguments.IsACKReceived)) && (!arguments.Quitting))
                    {                    
                        if (!arguments.IsACKReceived)
                        {
                            lateACKcntr += 1;
                            if (lateACKcntr >= 100)
                            {
                                if (serPort.IsOpen)
                                    serPort.Write(StateSend);
                                Thread.Sleep(100);
                            }
                        }
                    }

                    lateACKcntr = 0;
                    arguments.IsACKReceived = false;
                }
                AnimationSend.Abort();
            }

            private void fprocessClock(clockArgs cArgs)
            {
                FrameDimension dimension = new FrameDimension(cArgs.Image.FrameDimensionsList[0]);
                while (!cArgs.Quitting)
                {
                    if (cArgs.Update)
                    {
                        DateTime Now = DateTime.Now;
                        int x = (Now.Minute);
                        int y = (Now.Hour % 12);
                        cArgs.StringValue = y.ToString() + " - " + x.ToString();
                        cArgs.Image.SelectActiveFrame(dimension, y * 60 + x);
                        cArgs.OutImage = (Bitmap)cArgs.Image.Clone();
                        cArgs.Update = false;
                    }
                    Thread.Sleep(1);
                }
            }
        #endregion

        #region CALLBACK Functions ----------------------------------------------------------------------------
            private void btnSingleSend_Click(object sender, EventArgs e)
            {
                if (blackPixels == null)
                {
                    MessageBox.Show("Load and convert an image first!");
                    return;
                }

                if (!DataSend.IsAlive)
                    DataSend.Start();             /* start the thread dor the first time */
                else
                    dataSendArgs.IsDone = false;  /* restart again*/

            }
            private void btnAddAnimation_Click(object sender, EventArgs e)
            {
                animSendArgs.IsDone = true;
                lstFiles.Items.Clear();
            
                FolderBrowserDialog folder = new FolderBrowserDialog();
                folder.RootFolder = Environment.SpecialFolder.Desktop;
                if (!string.IsNullOrEmpty((string)key.GetValue(AnimFolder)))
                     folder.SelectedPath = key.GetValue(AnimFolder).ToString();

                DialogResult dr = folder.ShowDialog();
                if (dr == DialogResult.OK)
                {
                    foreach (var dir in  new DirectoryInfo(folder.SelectedPath).GetFiles())
                    {
                        if (ImageTypes.Contains(dir.Extension))
                        {                        
                            Bitmap bmp = Image.FromFile(dir.FullName) as Bitmap;
                               // AddBorder(ref bmp);
                                if (getBlackPixels(bmp).Count<10000)
                                    lstFiles.Items.Add(dir);
                            bmp.Dispose();
                        }
                    }

                    key.SetValue(AnimFolder, folder.SelectedPath);
                }

                animSendArgs.Update = true;
               //  animSendArgs.IsDone = false;
            }
            private void btnAminating_Click(object sender, EventArgs e)
            {
                if (key.GetValue(AnimFolder) == null)
                    return;

                if (!AnimationSend.IsAlive)
                {
                    AnimationSend.Start();                         /* start the thread for the first time */
                    btnAminating.Text = "Stop animating";
                    animSendArgs.IsDone = false;
                }
                else
                {
                    animSendArgs.IsDone = !animSendArgs.IsDone;    /* finish */

                    if (animSendArgs.IsDone)
                        btnAminating.Text = "Start animating";
                    else
                        btnAminating.Text = "Stop animating";
                }

                btnAddAnimation.Enabled = animSendArgs.IsDone;
            }
            private void btnProcess_Click(object sender, EventArgs e)
            {
                if (blackPixels != null)
                {
                    if ((blackPixels.Count == 0))
                        return;
                }
                else
                    return;

                FolderBrowserDialog folder = new FolderBrowserDialog();
                folder.RootFolder = Environment.SpecialFolder.Desktop;
                if (key.GetValue(OUTFolder) != null)
                    folder.SelectedPath = key.GetValue(OUTFolder).ToString();
                DialogResult dr = folder.ShowDialog();
                if (dr == DialogResult.OK)
                {
                    key.SetValue(OUTFolder, folder.SelectedPath);
                    /* write header file */
                    writeIntoFile(blackPixels, folder.SelectedPath, true);
                    /* write framed data into text file */
                    //writeIntoFile(blackPixels, folder.SelectedPath);
                    MessageBox.Show("Doné!");
                }

            }
            private void btn_load_Click(object sender, EventArgs e)
            {
                OpenFileDialog file = new OpenFileDialog();
                file.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
                file.Filter = "Image|*.bmp|Image|*.png";
                DialogResult dr = file.ShowDialog();
                if (dr == DialogResult.OK)
                {
                    selectedFile = file.FileName;

                    Bitmap bmp = Image.FromFile(selectedFile) as Bitmap;
                    AddBorder(ref bmp);
                    blackPixels = getBlackPixels(bmp);
                    bmp.Dispose();

                    if (blackPixels.Count == 0)
                    {
                        MessageBox.Show("No Data");
                        return;
                    }
                    createbackAgain(blackPixels, ref pBox);

                }
            }
            private void chkContinuousSend_CheckedChanged(object sender, EventArgs e)
            {
                tmr.Start();
                tmr.Enabled = chkContinuousSend.Checked;
            }
        
            private void Form1_FormClosing(object sender, FormClosingEventArgs e)
            {
                dataSendArgs.Quitting = true;
                animSendArgs.Quitting = true;
                ipd.Quitting = true;
                cArgs.Quitting = true;
            }
            private void serPort_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
            {
                string readBuffer = serPort.ReadExisting();

                if (readBuffer.Contains(handshakeReceive))
                {
                    ipd.Done(true);
                    this.Invoke((MethodInvoker)(() => this.Text = "Connected"));
                }
                else if (readBuffer.Contains(transferCompleted))
                {
                    animSendArgs.IsACKReceived = true;
                }
            }
            private void tmr_Tick(object sender, EventArgs e)
            {
                Bitmap bmp = new Bitmap(100, 100);
                using (Graphics g = Graphics.FromImage(bmp))
                {

                    g.CopyFromScreen
                    (
                        new Point(Cursor.Position.X, Cursor.Position.Y),
                        new Point(0, 0),
                        bmp.Size
                    );

                    ToGrayScale(bmp);
                    pBox1.Image = bmp;

                    blackPixels = getBlackPixels(bmp);

                }
                // bmp.Save(@"C:\Users\Adem\Desktop\XYOSCPlotter\imageToArray\ita\ggg.bmp");
                // bmp.Dispose();                            // dispose edilince çalışmıyor !!!!! neden?
                btnSingleSend_Click(this, EventArgs.Empty);
            }
            static int prevMinutes = int.MaxValue;
            private void tmrClock_Tick(object sender, EventArgs e)
            {
                if (prevMinutes != DateTime.Now.Minute)
                {
                    (tmrClock.Tag as clockArgs).Update = true;
                    while ((tmrClock.Tag as clockArgs).Update)
                    {
                        Thread.Sleep(1);
                    }

                    Bitmap bmp = (tmrClock.Tag as clockArgs).OutImage;
                    blackPixels = getBlackPixels(bmp);
                    //sendFrameSizeData(blackPixels);
                    btnSingleSend_Click(this, EventArgs.Empty);
                    //(tmrClock.Tag as clockArgs).OutImage.Save(@"C:\Users\Sapphire\Desktop\xyoscplotter\Images\splitTest\\" + (tmrClock.Tag as clockArgs).StringValue + ".bmp");

                    (tmrClock.Tag as clockArgs).OutImage.Dispose();
                    bmp.Dispose();

                    prevMinutes = DateTime.Now.Minute;
                }
            }
            private void chkClock_CheckedChanged(object sender, EventArgs e)
            {
                if (chkClock.Checked)
                    tmrClock.Start();
                else
                    tmrClock.Stop();

                prevMinutes = int.MaxValue;
            }
        #endregion

        #region FUNCTIONS ----------------------------------------------------------------------------
            private void        sendFrameSizeData(List<Point> _blackPixels)
            {
                if (!serPort.IsOpen)
                    return;

                byte[] frameArr = new byte[3];
                frameArr[0] = (byte)'F';
                frameArr[1] = (byte)(_blackPixels.Count / 256);
                frameArr[2] = (byte)(_blackPixels.Count % 256);
                serPort.Write(frameArr, 0, frameArr.Length);
            }
            private Bitmap      CropImage(Image originalImage, Rectangle sourceRectangle, Rectangle? destinationRectangle = null)
            {
                if (destinationRectangle == null)
                {
                    destinationRectangle = new Rectangle(Point.Empty, sourceRectangle.Size);
                }

                var croppedImage = new Bitmap(destinationRectangle.Value.Width, destinationRectangle.Value.Height);
                using (var graphics = Graphics.FromImage(croppedImage))
                {
                    graphics.DrawImage(originalImage, destinationRectangle.Value, sourceRectangle, GraphicsUnit.Pixel);
                }
                return croppedImage;
            }
            private void        AddBorder(ref Bitmap bmp)
            {
                Bitmap _bmp = new Bitmap(bmp);

                int width = _bmp.Width;
                int heigth = _bmp.Height;

                using (Graphics gr = Graphics.FromImage(_bmp))
                {
                    gr.DrawRectangle(new Pen(Color.Black), new Rectangle(0, 0, _bmp.Width - 1, _bmp.Height - 1));
                    gr.DrawImage(_bmp, 0, 0);
                }
                bmp = new Bitmap(_bmp);
                _bmp.Dispose();

            }
            public void         ToGrayScale(Bitmap SourceImage)
            {
                using (Graphics gr = Graphics.FromImage(SourceImage)) // SourceImage is a Bitmap object
                {
                    var gray_matrix = new float[][] {
                    new float[] { 0.299f, 0.299f, 0.299f, 0, 0 },
                    new float[] { 0.587f, 0.587f, 0.587f, 0, 0 },
                    new float[] { 0.114f, 0.114f, 0.114f, 0, 0 },
                    new float[] { 0,      0,      0,      1, 0 },
                    new float[] { 0,      0,      0,      0, 1 }
                };

                    var ia = new System.Drawing.Imaging.ImageAttributes();
                    ia.SetColorMatrix(new System.Drawing.Imaging.ColorMatrix(gray_matrix));
                    ia.SetThreshold(0.5F); // Change this threshold as needed
                    var rc = new Rectangle(0, 0, SourceImage.Width, SourceImage.Height);
                    gr.DrawImage(SourceImage, rc, 0, 0, SourceImage.Width, SourceImage.Height, GraphicsUnit.Pixel, ia);
                }
            }
            private List<Point> getBlackPixels(Bitmap bitmap)
            {
                List<Point> lst = new List<Point>();
                for (int x = 0; x < bitmap.Width; x++)
                {
                    for (int y = 0; y < bitmap.Height; y++)
                    {
                        Color c = bitmap.GetPixel(x, y);
                        if ((c.R == 0) && (c.G == 0) && (c.B == 0))
                            lst.Add(new Point(x, y));
                    }
                }
                return lst;
            }        
            private void        createbackAgain(List<Point> list, ref PictureBox p)
            {
                Point maxs = returnMaxs(list);
                Bitmap bmp = new Bitmap(maxs.X + 1, maxs.Y + 1);

                foreach (Point pl in list)
                {
                    bmp.SetPixel(pl.X, pl.Y, Color.Red);
                }
                p.Image = bmp;
            }
            private Point       returnMaxs(List<Point> lst)
            {
                Point p = new Point(-1, -1);

                // getmax
                foreach (Point pl in lst)
                {
                    if (pl.X > p.X)
                        p.X = pl.X;
                    if (pl.Y > p.Y)
                        p.Y = pl.Y;
                }

                return p;
            }
            private List<Point> ImageCasting(List<Point> _list, string castVal, bool revertX, bool revertY,bool swapXY)
            {
                List<Point> retVal = new List<Point>();
                MinMax minmax = returnMinMax(_list);

                float OL = 0;
                float OH = 0;
                if (string.IsNullOrEmpty(castVal))
                {
                    OL = 0;
                    OH = 4095;
                }
                else
                {
                    OL = (float)Convert.ToDouble(MapText.Split(';')[0]);
                    OH = (float)Convert.ToDouble(MapText.Split(';')[1]);
                }


                for (int i = 0; i < _list.Count; i++)
                {
                    Point p = new Point();

                    p.X = (int)MAP(_list[i].X, (float)minmax.GetMin, (float)minmax.GetMax, OL, OH);
                    if (revertX)
                        p.X = (int)(OH - MAP(_list[i].X, (float)minmax.GetMin, (float)minmax.GetMax, OL, OH));

                    p.Y = (int)MAP(_list[i].Y, (float)minmax.GetMin, (float)minmax.GetMax, OL, OH);
                    if (revertY)
                        p.Y = (int)(OH - MAP(_list[i].Y, (float)minmax.GetMin, (float)minmax.GetMax, OL, OH));

                    if (swapXY)
                    {
                        int temp = p.X;
                        p.X = p.Y;
                        p.Y = temp;
                    }

                    retVal.Add(p);
                }

                key.SetValue(DAClimits, castVal);

                return retVal;
            }
            private float       MAP(float X, float InputLow, float InputHigh, float OutputLow, float OutputHigh)
            {
                float retval = 0.0F;
                float M = ((OutputHigh - OutputLow) / (InputHigh - InputLow));
                retval = (X - InputLow) * M + OutputLow;
                return retval;
            }
            private MinMax      returnMinMax(List<Point> list)
            {
                MinMax mm = new MinMax(int.MinValue, int.MinValue);
                Point p = new Point(int.MinValue, int.MinValue);

                // getmax
                foreach (Point pl in list)
                {
                    if (pl.X > p.X)
                        p.X = pl.X;
                    if (pl.Y > p.Y)
                        p.Y = pl.Y;
                }
                if (p.X > p.Y)
                    mm.SetMax = p.X;
                else
                    mm.SetMax = p.Y;

                //getmin
                p = new Point(int.MaxValue, int.MaxValue);
                foreach (Point pl in list)
                {
                    if (pl.X < p.X)
                        p.X = pl.X;
                    if (pl.Y < p.Y)
                        p.Y = pl.Y;
                }
                if (p.X < p.Y)
                    mm.SetMin = p.X;
                else
                    mm.SetMin = p.Y;

                return mm;
            }
            private void        writeIntoFile(List<Point> list, string path)
            {

                List<Point> listTemp;

                listTemp = ImageCasting(list, MapText, chkRevX.Checked, chkRevY.Checked, chkBoxSwapXY.Checked);

                using (StreamWriter sw = new StreamWriter(path + @"\Ascii.txt"))
                {
                    char[] arr = new char[3] { char.MinValue, char.MinValue, char.MinValue };
                    foreach (Point p in listTemp)
                    {
                        arr[0] = 'D';
                        arr[1] = (char)p.X;
                        arr[2] = (char)p.Y;
                        sw.WriteLine(arr);
                    }
                }
            }
            private void        writeIntoFile(List<Point> list, string path, bool ParseMap)
            {
                List<Point> listTemp;

                if (ParseMap)
                {
                    listTemp = ImageCasting(list, MapText, chkRevX.Checked, chkRevY.Checked, chkBoxSwapXY.Checked);
                }
                else
                {
                    listTemp = list;
                }

                using (StreamWriter sw = new StreamWriter(path + @"\arrays.h"))
                {
                    StringBuilder XArr = new StringBuilder();
                    StringBuilder YArr = new StringBuilder();

                    XArr.AppendLine("#ifndef __ARRAYS_H__");
                    XArr.AppendLine("#define __ARRAYS_H__");
                    XArr.AppendLine("# include <stdint.h>");
                    XArr.AppendLine("#define ARRAYLENGTHTemp " + list.Count.ToString());
                    XArr.AppendLine("extern uint8_t XArrTemp[];");
                    XArr.AppendLine("extern uint8_t YArrTemp[];");

                    XArr.Append("uint8_t XArrTemp[ARRAYLENGTHTemp]={");
                    YArr.Append("uint8_t YArrTemp[ARRAYLENGTHTemp]={");
                    for (int i = 0; i < listTemp.Count; i++)
                    {
                        if (i < (listTemp.Count - 1))
                        {
                            XArr.Append(listTemp[i].X); XArr.Append(",");
                            YArr.Append(listTemp[i].Y); YArr.Append(",");
                        }
                        else
                        {
                            XArr.Append(listTemp[i].X);
                            YArr.Append(listTemp[i].Y);
                        }
                    }
                    XArr.Append("};");
                    YArr.AppendLine("};");
                    YArr.AppendLine("#endif");

                    sw.WriteLine(XArr);
                    sw.WriteLine(YArr);
                }
            }
        #endregion
    }

    public class MinMax
    {
        private int _Min = 0;
        private int _Max = 0;

        public MinMax(int min, int max)
        {
            this._Min = min;
            this._Max = max;
        }
        public MinMax()
        {
            this._Min = 0;
            this._Max = 0;
        }

        public int SetMin { set { this._Min = value; } }
        public int SetMax { set { this._Max = value; } }
        public int GetMin { get { return this._Min; } }
        public int GetMax { get { return this._Max; } }
    }
    public class SendArgs : EventArgs
    {
        private bool _done=false;
        private bool _ACKReceived = false;
        private bool _update = false;
        private bool _quit = false;

        public SendArgs(bool val)
        {
            _done = val;
        }


        public bool IsDone
        {
            get { return _done; }
            set { _done = value; }
        }

        public bool IsACKReceived
        {
            get { return _ACKReceived; }
            set { _ACKReceived = value; }
        }

        public bool Update
        {
            get { return _update; }
            set { _update = value; }
        }

        public bool Quitting
        {
            get { return _quit; }
            set { _quit = value; }
        }
    }
    public class IsProcessDone : EventArgs
    {
        private bool _val = false;
        private string _portName=string.Empty;
        private bool _quit = false;

        public IsProcessDone(bool val,string pname)
        {
            _val = val;
            _portName = pname;
        }

        public void Done(bool val)
        {
            _val = val;
        }

        public bool IsDone
        {
            get { return _val; }
        }

        public void SetPortName(string val)
        {
            _portName = val;
        }

        public string getPortName
        {
            get { return _portName; }
        }

        public bool Quitting
        {
            get { return _quit; }
            set { _quit = value; }
        }


    }
    public class clockArgs : EventArgs
    {
        private bool _update = false;
        private bool _quit = false;
        private Bitmap _image;
        private Bitmap _outImage;
        private string _stringValue = string.Empty;

        public clockArgs(bool quit, bool update,  ref Bitmap img)
        {
            _update = quit;
            _quit = update;
            _image = img;
        }

        public bool Update
        {
            get { return _update; }
            set { _update = value; }
        }

        public bool Quitting
        {
            get { return _quit; }
            set { _quit = value; }
        }

        public Bitmap Image
        {
            get { return _image; }
            set { _image = new Bitmap(value); }
        }

        public Bitmap OutImage
        {
            get { return _outImage; }
            set { _outImage = new Bitmap(value); }
        }

        public string StringValue
        {
            get { return _stringValue; }
            set { _stringValue = value; }
        }

    }
}
