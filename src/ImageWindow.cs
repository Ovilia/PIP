using System;
using System.Drawing;
using System.Windows.Forms;

namespace PIP
{
  class ImageWindow : Form
  {
    private Bitmap imageBitmap;
    public Bitmap ImageBitmap
    {
      get
      {
        return imageBitmap;
      }
    }

    private PictureBox pictureBox;

    private string imageFileName;
    public string ImageFileName
    {
      get
      {
        return imageFileName;
      }
    }

    /// <summary>
    /// Only one ImageWindow is focused in imageWindowList
    /// </summary>
    public bool isFocused{get;set;}


    /// <summary>
    /// Default constructor
    /// </summary>
    public ImageWindow()
    {
      InitializeComponent();
      isFocused = true;
    }

    /// <summary>
    /// Constructor with given image file name
    /// </summary>
    /// <param name="imageFileName">File name of the image</param>
    public ImageWindow(string imageFileName)
    {
      InitializeComponent();
      isFocused = true;
      loadImage(imageFileName);
    }

    /// <summary>
    /// Constructor with given bitmap
    /// </summary>
    /// <param name="bitmap">Bitmap of the image</param>
    public ImageWindow(Bitmap bitmap)
    {
      InitializeComponent();
      isFocused = true;
      imageBitmap = bitmap;
      this.pictureBox.Image = imageBitmap;
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
        LogSystem.getInstance().writeLog(LogSystem.ActionType.OPEN_FILE);
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
      this.GotFocus += new System.EventHandler(this.ImageWindow_GotFocus);
      this.Resize += new System.EventHandler(this.ImageWindow_Resize);
      ((System.ComponentModel.ISupportInitialize)(this.pictureBox)).EndInit();
      this.ResumeLayout(false);

    }

    private void ImageWindow_Resize(object sender, EventArgs e)
    {
      this.pictureBox.Size = this.ClientSize;
    }

    private void ImageWindow_GotFocus(object sender, EventArgs e)
    {
      MainForm.windowManager.focusImageWindow(this);
    }
  }
}
