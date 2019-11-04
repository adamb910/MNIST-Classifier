using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Text;

namespace WpfInkCavasSaveImage
{
    public class Vector
    {
        public float x, y = 0;

        public Vector()
        {
            x = 0; y = 0;
        }
        public Vector(int x_, int y_)
        {
            x = x_; y = y_;
        }

        public static Vector Convert(int i, int w, int h)
        {
            int sor = i / w;
            int oszlop = i % w;
            return new Vector(oszlop, sor);
        }

        public void Print()
        {
            Debug.WriteLine($"({x},{y});");
        }
    }

    class InputPipeline
    {
        Bitmap image;
        int targetWidth = 20;
        int targetHeight = 20;
        Brush background = new SolidBrush(Color.White);

        public InputPipeline(Bitmap b)
        {
            image = b;
        }

        public Bitmap Transform()
        {
            Vector highest = FindHighestPoint();
            Vector lowest = FindLowestPoint();
            Vector rightest = FindRightestPoint();
            Vector leftest = FindLeftestPoint();
            Debug.WriteLine("\n\n\n\nPARAMS: ");
            highest.Print();
            lowest.Print();
            rightest.Print();
            leftest.Print();

            //CROPPING
            Rectangle cropRect = new Rectangle((int)leftest.x, (int)highest.y, (int)(rightest.x-leftest.x), (int)(lowest.y-highest.y));
            Bitmap src = image;
            Bitmap target = new Bitmap(cropRect.Width, cropRect.Height);

            using (Graphics g = Graphics.FromImage(target))
            {
                g.DrawImage(src, new Rectangle(0, 0, target.Width, target.Height),
                                 cropRect,
                                 GraphicsUnit.Pixel);
            }

            //target.Save("CROPTEST.bmp");
            image = target;




            var bmp = new Bitmap((int)targetWidth, (int)targetHeight);
            var graph = Graphics.FromImage(bmp);

            // uncomment for higher quality output
            graph.InterpolationMode = InterpolationMode.High;
            graph.CompositingQuality = CompositingQuality.HighQuality;
            graph.SmoothingMode = SmoothingMode.AntiAlias;


            float scale = Math.Min((float)targetWidth / (float)image.Width, (float)targetHeight / (float)image.Height);
            int scaleWidth = (int)(image.Width * scale);
            int scaleHeight = (int)(image.Height * scale);

            graph.FillRectangle(background, new RectangleF(-1, -1, targetWidth+1, targetHeight+1));
            graph.DrawImage(image, ((int)targetWidth - scaleWidth) / 2, ((int)targetHeight - scaleHeight) / 2, scaleWidth, scaleHeight);
            //bmp = new Bitmap(image, new Size(scaleHeight, scaleWidth));
            //bmp.Save("TRANSFORMTESTNEW.bmp");
            return bmp; // CHANGE
        }


        public static Vector CenterOfMass(float[] input, int w, int h)
        {
            Vector avg = new Vector();
            int notzero = 0;
            for (int i = 0; i < input.Length; i++)
            {
                if (input[i] > 0)
                {
                    Vector conv = Vector.Convert(i, w, h);
                    //Debug.WriteLine("added to avg: " + conv.x + " " + conv.y);
                    //Debug.WriteLine("input[i] " + input[i]);
                    avg.x += conv.x;
                    avg.y += conv.y;
                    notzero++;
                }
            }
            avg.x /= notzero;
            avg.y /= notzero;
            return avg;
        }

        //TOP LEFT CORNER IS 0,0....
        public Vector FindHighestPoint()
        {
            Vector highest = new Vector(int.MaxValue, int.MaxValue);
            for (int j = 10; j < image.Height; j++)
            {
                for (int i = 10; i < image.Width; i++)
                {
                    if ((float)Math.Abs(image.GetPixel(i, j).GetBrightness() - 1) > 0.2f)
                    {
                        if (j < highest.y)
                        {
                            highest.x = i;
                            highest.y = j;
                        }
                    }                   
                }
            }
            return highest;
        }
        //low as in bigger number, lower down on the coordinate system, TOP LEFT CORNER IS 0,0....
        public Vector FindLowestPoint()
        {
            Vector lowest = new Vector(-1, -1);
            for (int j = 0; j < image.Height; j++)
            {
                for (int i = 0; i < image.Width; i++)
                {
                    if ((float)Math.Abs(image.GetPixel(i, j).GetBrightness() - 1) > 0.2f)
                    {
                        if (j > lowest.y)
                        {
                            lowest.x = i;
                            lowest.y = j;
                        }
                    }
                }
            }
            return lowest;
        }
        public Vector FindRightestPoint()
        {
            Vector rightest = new Vector(-1, -1);
            for (int j = 10; j < image.Height; j++)
            {
                for (int i = 0; i < image.Width; i++)
                {
                    if ((float)Math.Abs(image.GetPixel(i, j).GetBrightness() - 1) > 0.2f)
                    {
                        if (i > rightest.x)
                        {
                            rightest.x = i;
                            rightest.y = j;
                        }
                    }
                }
            }
            return rightest;
        }
        public Vector FindLeftestPoint()
        {
            Vector leftest = new Vector(int.MaxValue, int.MaxValue);
            for (int j = 10; j < image.Height; j++)
            {
                for (int i = 10; i < image.Width; i++)
                {
                    if ((float)Math.Abs(image.GetPixel(i, j).GetBrightness() - 1) > 0.2f)
                    {
                        if (i < leftest.x)
                        {
                            leftest.x = i;
                            leftest.y = j;
                        }
                    }
                }
            }
            return leftest;
        }


    }
}
