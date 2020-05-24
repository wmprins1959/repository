using System;
using System.Windows.Forms;

namespace Tali
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application, wow
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}

