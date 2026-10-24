// Storage.cs -- XNA's StorageDevice and StorageContainer on the PC's disk: one device (no selector to show), and a
// container is a folder, %LOCALAPPDATA%\CodeBreaker\<game>\<container display name>, as a console kept a title's
// saves in its own space on the chosen device.
using System;
using System.IO;
using Microsoft.Xna.Framework;

namespace Microsoft.Xna.Framework.Storage
{
    public partial class StorageDevice
    {
        internal static readonly StorageDevice Local = new StorageDevice();
        protected StorageDevice() { }

        internal static string Root
        {
            get
            {
                string game = XnaPort.Runtime.Game?.GetType().Assembly.GetName().Name ?? "game";
                string root = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "CodeBreaker", game);
                Directory.CreateDirectory(root);
                return root;
            }
        }

        public long FreeSpace { get { return new DriveInfo(Path.GetPathRoot(Root)).AvailableFreeSpace; } }
        public bool IsConnected { get { return true; } }
        public long TotalSpace { get { return new DriveInfo(Path.GetPathRoot(Root)).TotalSize; } }
#pragma warning disable 67
        public static event EventHandler<EventArgs> DeviceChanged;
#pragma warning restore 67
        public static IAsyncResult BeginShowSelector(PlayerIndex player, AsyncCallback callback, object state) => XnaPort.Completed.Call(callback, state, Local);
        public static IAsyncResult BeginShowSelector(PlayerIndex player, int sizeInBytes, int directoryCount, AsyncCallback callback, object state) => XnaPort.Completed.Call(callback, state, Local);
        public static IAsyncResult BeginShowSelector(AsyncCallback callback, object state) => XnaPort.Completed.Call(callback, state, Local);
        public static IAsyncResult BeginShowSelector(int sizeInBytes, int directoryCount, AsyncCallback callback, object state) => XnaPort.Completed.Call(callback, state, Local);
        public static StorageDevice EndShowSelector(IAsyncResult result) { return Local; }
        public IAsyncResult BeginOpenContainer(string displayName, AsyncCallback callback, object state) => XnaPort.Completed.Call(callback, state, new StorageContainer(this, displayName));
        public StorageContainer EndOpenContainer(IAsyncResult result) { return XnaPort.Completed.Value<StorageContainer>(result); }
        public void DeleteContainer(string titleName) { string p = Path.Combine(Root, titleName); if (Directory.Exists(p)) Directory.Delete(p, true); }
    }

    public partial class StorageContainer : IDisposable
    {
        readonly string folder;
        internal StorageContainer(StorageDevice device, string name)
        {
            StorageDevice = device;
            DisplayName = name;
            folder = Path.Combine(StorageDevice.Root, string.Join("_", name.Split(Path.GetInvalidFileNameChars())));
            Directory.CreateDirectory(folder);
        }
        protected StorageContainer() { }

        string At(string path) => Path.Combine(folder, path.Replace('\\', Path.DirectorySeparatorChar).TrimStart(Path.DirectorySeparatorChar));

        public string DisplayName { get; private set; }
        public StorageDevice StorageDevice { get; private set; }
        public bool IsDisposed { get; private set; }
        public event EventHandler<EventArgs> Disposing;
        public bool DirectoryExists(string directory) => Directory.Exists(At(directory));
        public bool FileExists(string file) => File.Exists(At(file));
        public void CreateDirectory(string directory) => Directory.CreateDirectory(At(directory));
        public Stream CreateFile(string file) { Directory.CreateDirectory(Path.GetDirectoryName(At(file))); return File.Create(At(file)); }
        public void DeleteDirectory(string directory) => Directory.Delete(At(directory), true);
        public Stream OpenFile(string file, FileMode fileMode) => OpenFile(file, fileMode, fileMode == FileMode.Open ? FileAccess.Read : FileAccess.ReadWrite);
        public Stream OpenFile(string file, FileMode fileMode, FileAccess fileAccess) => OpenFile(file, fileMode, fileAccess, FileShare.None);
        public Stream OpenFile(string file, FileMode fileMode, FileAccess fileAccess, FileShare fileShare)
        {
            if (fileMode != FileMode.Open) Directory.CreateDirectory(Path.GetDirectoryName(At(file)));
            return new FileStream(At(file), fileMode, fileAccess, fileShare);
        }
        public void DeleteFile(string file) => File.Delete(At(file));
        public string[] GetDirectoryNames() => GetDirectoryNames("*");
        public string[] GetDirectoryNames(string searchPattern) => Array.ConvertAll(Directory.GetDirectories(folder, searchPattern), Path.GetFileName);
        public string[] GetFileNames() => GetFileNames("*");
        public string[] GetFileNames(string searchPattern) => Array.ConvertAll(Directory.GetFiles(folder, searchPattern), Path.GetFileName);
        public void Dispose() { if (!IsDisposed) { Disposing?.Invoke(this, EventArgs.Empty); IsDisposed = true; } }
    }
}
