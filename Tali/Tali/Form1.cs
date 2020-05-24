// by W.M. Prins

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Tali
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();  // improved worthless comment
        }

        private void textBoxInput_DragDrop(object sender, DragEventArgs e)
        {
            string[] paths = (string[])e.Data.GetData("FileNameW");

            //retrieve the path of first file or any other as you want
            string pwd = paths[0];
            textBoxInput.Text = pwd;

            bool is_dir = System.IO.Directory.Exists(pwd);
            if (!is_dir)
            {
                int last_occurrence = pwd.LastIndexOf("\\");
                if (last_occurrence != -1)
                {
                    pwd = pwd.Substring(0, last_occurrence + 1);
                }
                is_dir = System.IO.Directory.Exists(pwd);
            }
            if (is_dir)
            {
                textBoxInput.Text = pwd;
            }
            check_inputs();
        }

        private void check_inputs()
        {
            bool is_dir = System.IO.Directory.Exists(textBoxInput.Text);
            bool name_ok = (textBoxName.Text.Length > 0) && !textBoxName.Text.Contains("\\");
            buttonGo.Enabled = is_dir && name_ok;
        }

        private void textBoxInput_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.Copy;
        }

        private void buttonGo_Click(object sender, EventArgs e)
        {
            string new_dir = textBoxInput.Text + "\\" + textBoxName.Text;
            var dir_info = System.IO.Directory.CreateDirectory(new_dir);
            if (dir_info.Exists)
            {
                var files = System.IO.Directory.GetFiles(textBoxInput.Text);
                foreach (var file in files)
                {
                    if (file.Contains(textBoxName.Text))
                    {
                        string dest = new_dir + "\\" + reduced_name(file);
                        dest = dest.Replace("\\\\", "\\");
                        dest = dest.Replace("\\\\", "\\");
                        // MessageBox.Show(file + "move to " + dest);
                        try
                        {
                        System.IO.File.Move(file, dest);
                        }
                        catch (Exception)
                        {
                            MessageBox.Show("Cannot move " + file + " to " + dest);
                        }
                    }
                }
            }
        }

        private void textBoxInput_TextChanged(object sender, EventArgs e)
        {
            check_inputs();
        }

        private static string reduced_name(string path)
        {
            int last_occurrence = path.LastIndexOf("\\");
            if (last_occurrence != -1)
            {
                string reduced_name = path.Substring(last_occurrence + 1);
                return reduced_name;
            }
            return path;
        }

        private void textBoxName_DragDrop(object sender, DragEventArgs e)
        {
            string[] paths = (string[])e.Data.GetData("FileNameW");
            string name = paths[0]; // path of the first file
            
            int last_occurrence = name.LastIndexOf("\\");
            if (last_occurrence != -1)
            {
                string reduced_name = name.Substring(last_occurrence + 1);
                name = reduced_name;
            }

            while ((last_occurrence = name.LastIndexOf(".")) != -1)
            {
                string reduced_name = name.Substring(0, last_occurrence);
                name = reduced_name;
            }
            
            textBoxName.Text = name;
            check_inputs();
        }

        private void textBoxName_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.Copy;
        }

        private void textBoxName_TextChanged(object sender, EventArgs e)
        {
            check_inputs();
        }
    }
}
