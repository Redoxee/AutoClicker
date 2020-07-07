using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;

namespace SWIClientReleaseTool
{
    class Program
    {
        static void Main(string[] args)
        {

            if (args.Length == 0)
            {
                Console.WriteLine("No release folder specified.");
                return;
            }

            bool isDryRun = true;
            if (args.Contains("/D"))
            {
                isDryRun = false;
            }

            string releaseFolder = args[args.Length -1];
            if (!Directory.Exists(releaseFolder))
            {
                Console.WriteLine($"Release folder nor found {releaseFolder}");
                return;
            }

            HashSet<string> authorizedFile = new HashSet<string>();
            string[] authorizedFilesRaw = ressources.AuthorizeFiles.Split("\n");
            foreach(string fileName in authorizedFilesRaw)
            {
                if (string.IsNullOrEmpty(fileName))
                {
                    continue;
                }

                string file = fileName.Trim();
                if (!authorizedFile.Contains(file))
                {
                    authorizedFile.Add(file);
                }
            }

            var allPresentFiles = Directory.GetFiles(releaseFolder, "*", SearchOption.AllDirectories);
            if (allPresentFiles.Length > 500)
            {
                Console.WriteLine($"Are you sure this is the correct directory ? {releaseFolder}");
            }

            string msDllName = "msvcp140.dll";
            bool msDllFound = false;

            foreach (string presentFile in allPresentFiles)
            {
                int fileNameStart = presentFile.LastIndexOf("\\");
                string fileName = presentFile.Substring(fileNameStart + 1);
                if (!authorizedFile.Contains(fileName))
                {
                    Console.WriteLine($"Deleting : {fileName}");
                    if (!isDryRun)
                    {
                        File.Delete(presentFile);
                    }
                }
                else
                {
                    if (fileName == msDllName)
                    {
                        msDllFound = true;
                    }

                    Console.WriteLine($"Preserving : {fileName}");
                }
            }

            var allDirectories = Directory.GetDirectories(releaseFolder);
            foreach (string directory in allDirectories)
            {
                if (Directory.GetFiles(directory).Length == 0)
                {
                    Directory.Delete(directory);
                }
            }

            if (!msDllFound)
            {
                if (!isDryRun)
                {
                    FileStream msStream = File.Create(releaseFolder + "\\" + msDllName);
                    msStream.Write(ressources.msvcp140);
                    msStream.Close();
                }
            }

            int folderEnd = releaseFolder.LastIndexOf("\\");
            string folderName = releaseFolder.Substring(0, folderEnd);
            string zipName = releaseFolder;
            zipName += ".zip";

            // TODO : find a way to generalize this.
            string zipBin = "E:\\Programmes_\\7Zip\\7z.exe";
            string zipCommand = $"{zipBin} a {zipName} {folderName}";

            Console.WriteLine($"Zipping to {zipName}");
            if (!isDryRun)
            {
                Console.WriteLine("Command");
                Console.WriteLine(zipCommand);
                System.Diagnostics.Process.Start(zipCommand);
            }
        }
    }
}
