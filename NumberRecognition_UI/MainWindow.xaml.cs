using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Runtime.InteropServices;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;

namespace WpfInkCavasSaveImage
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        [DllImport(@"CLIWrapper.dll", CallingConvention = CallingConvention.Cdecl,
       CharSet = CharSet.Unicode)]
        public static extern void CreateNetwork();

        [DllImport(@"CLIWrapper.dll", CallingConvention = CallingConvention.Cdecl,
          CharSet = CharSet.Unicode)]
        public static extern int Forward([In, MarshalAs(UnmanagedType.LPArray, SizeConst = 784)] float[] input);

        [DllImport(@"CLIWrapper.dll", CallingConvention = CallingConvention.Cdecl,
         CharSet = CharSet.Unicode)]
        public static extern int ForwardWithNormalization([In, MarshalAs(UnmanagedType.LPArray, SizeConst = 400)] float[] input);

        public MainWindow()
        {
            InitializeComponent();
          
            CreateNetwork();
        }     

        private void btnSaveInkCanvas(object sender, RoutedEventArgs e)
        {
         
            RenderTargetBitmap rtb = new RenderTargetBitmap(/*(int)inkCanvas1.Width*//*560*/400, /*(int)inkCanvas1.Height*/400, 96d, 96d, PixelFormats.Default);
            System.Windows.Shapes.Rectangle fillBackground = new System.Windows.Shapes.Rectangle
            {
                Width = 560,
                Height = 560,
                Fill = System.Windows.Media.Brushes.White
            };
            rtb.Render(fillBackground);
            rtb.Render(inkCanvas1);
            DrawingVisual dvInk = new DrawingVisual();
            DrawingContext dcInk = dvInk.RenderOpen();
            dcInk.DrawRectangle(inkCanvas1.Background, null, new Rect(0d, 0d, inkCanvas1.Width, inkCanvas1.Height));
            foreach (System.Windows.Ink.Stroke stroke in inkCanvas1.Strokes)
            {
                stroke.Draw(dcInk);
            }
            dcInk.Close();

            MemoryStream stream = new MemoryStream();
            BitmapEncoder encoder = new BmpBitmapEncoder();
            encoder.Frames.Add(BitmapFrame.Create(rtb));
            encoder.Save(stream);

            Bitmap bitmap = new Bitmap(stream);



            InputPipeline pipe = new InputPipeline(bitmap);
            Bitmap ered = pipe.Transform();
            ered.Save("transformtest.png");
            bitmap = ered;



            int o = 0;
            Debug.WriteLine("height " + bitmap.Height);
            Debug.WriteLine("width " + bitmap.Width);
            int incr = 20;
            float[] pixels = new float[bitmap.Height * bitmap.Width];
            for (int i = 0; i < bitmap.Height; i++)
            {
                for (int j = 0; j < bitmap.Width; j++)
                {

                    if (j == 0 || i == 0)
                        pixels[o] = 0;
                    else
                        if ((float)Math.Abs(bitmap.GetPixel(j, i).GetBrightness() - 1) > 0.05)
                            pixels[o] = (float)Math.Abs(bitmap.GetPixel(j, i).GetBrightness() - 1);
                    Debug.Write(String.Format("{0:0.00}", pixels[o])+ " ");
                    o++;
                    
                }
                Debug.WriteLine("");

            }
            Vector com = InputPipeline.CenterOfMass(pixels,bitmap.Height, bitmap.Width);
            Debug.WriteLine("center of mass: (" + com.x + ", " + com.y + ")");
            int result = ForwardWithNormalization(pixels);
            ShowResult(result);
            Debug.WriteLine("result " + result);


            //SAVE FOR DEBUG
            FileStream fs = File.Open("testimage.png", FileMode.OpenOrCreate);//save bitmap to file
            System.Windows.Media.Imaging.JpegBitmapEncoder encoder1 = new JpegBitmapEncoder();
            encoder1.Frames.Add(BitmapFrame.Create(rtb));
            encoder1.Save(fs);
            fs.Close();
        }

        void ShowResult(int res)
        {
            resultText.Text = res.ToString();
        }

        private void btnSaveImage_Click(object sender, RoutedEventArgs e)
        {
            inkCanvas1.Strokes.Clear();

        }
    }
}
