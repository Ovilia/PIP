using System;
using System.Drawing;
using System.Windows.Forms;

namespace PIP
{
  class ImageWindow : Form
  {
    private Bitmap imageBitmap;
    private PictureBox pictureBox;
    private string imageFileName;


    /// <summary>
    /// Default constructor
    /// </summary>
    public ImageWindow()
    {
      InitializeComponent();
    }

    /// <summary>
    /// Constructor with given image file name
    /// </summary>
    /// <param name="imageFileName">File name of the image</param>
    public ImageWindow(string imageFileName)
    {
      InitializeComponent();
      loadImage(imageFileName);
    }

    /// <summary>
    /// Open image with private imageFileName,
    /// show image in ImageWindow
    /// </summary>
    /// <returns>If load successfully</returns>
    public bool loadImage()
    {
      if (imageFileName == null)
      {
        return false;
      }
      try
      {
        imageBitmap = new Bitmap(imageFileName);
        this.pictureBox.Image = imageBitmap;
        this.Text = imageFileName;
        return true;
      }
      catch (Exception)
      {
        MessageBox.Show("Fail to load image: " + imageFileName,
          "Error in loading image",
          MessageBoxButtons.OK);
      }
      return false;
    }

    /// <summary>
    /// Load image with given name,
    /// call loadImage()
    /// </summary>
    /// <param name="imageFileName">New name of image file</param>
    /// <returns>If load successfully</returns>
    public bool loadImage(string imageFileName)
    {
      this.imageFileName = imageFileName;
      return loadImage();
    }

    private void InitializeComponent()
    {
      this.pictureBox = new System.Windows.Forms.PictureBox();
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox)).BeginInit();
      this.SuspendLayout();
      // 
      // pictureBox
      // 
      this.pictureBox.Location = new System.Drawing.Point(0, 0);
      this.pictureBox.Name = "pictureBox";
      this.pictureBox.Size = new System.Drawing.Size(484, 462);
      this.pictureBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
      this.pictureBox.TabIndex = 0;
      this.pictureBox.TabStop = false;
      // 
      // ImageWindow
      // 
      this.ClientSize = new System.Drawing.Size(484, 462);
      this.Controls.Add(this.pictureBox);
      this.Name = "ImageWindow";
      this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
      this.Text = "Image window";
      this.Resize += new System.EventHandler(this.ImageWindow_Resize);
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox)).EndInit();
      this.ResumeLayout(false);

    }

    private void ImageWindow_Resize(object sender, EventArgs e)
    {
      this.pictureBox.Size = this.ClientSize;
    }
  }
}
