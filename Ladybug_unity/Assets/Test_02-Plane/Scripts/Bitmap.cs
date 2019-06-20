using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using UnityEngine;

public class Bitmap
{
    [StructLayout(LayoutKind.Sequential, Pack = 2)]
    struct BitmapFileHeader
    {
        public ushort type;
        public uint size;
        public ushort reserved1;
        public ushort reserved2;
        public uint off_bits;
    };

    [StructLayout(LayoutKind.Sequential, Pack = 2)]
    struct BitmapInformationHeader
    {
        public uint size;
        public uint width;
        public uint height;
        public ushort planes;
        public ushort bit_count;
        public uint compression;
        public uint size_image;
        public uint x_pels_per_meter;
        public uint y_pels_per_meter;
        public uint clr_used;
        public uint clr_important;
    };

    public int width;
    public int height;
    private Color32[] buffer;

    public Bitmap(int width, int height)
    {
        this.width = width;
        this.height = height;
        buffer = new Color32[width * height];
    }

    public void SetPixel(int x, int y, Color color)
    {
        Color32 c = buffer[y * width + x];
        c.r = (byte)(color.r * 255);
        c.g = (byte)(color.g * 255);
        c.b = (byte)(color.b * 255);
        c.a = (byte)(color.a * 255);
        buffer[y * width + x] = c;
    }

    public void SetPixel(int x, int y, Color32 color)
    {
        buffer[y * width + x] = color;
    }

    public void SetPixels(Color32[] pixels)
    {
        if (pixels.Length == buffer.Length)
        {
            buffer = pixels;
        }
    }

    private static byte[] GetBytes<T>(T data) where T : struct
    {
        int structSize = Marshal.SizeOf(typeof(T));
        byte[] buffer = new byte[structSize];
        GCHandle handle = GCHandle.Alloc(buffer, GCHandleType.Pinned);
        Marshal.StructureToPtr(data, handle.AddrOfPinnedObject(), false);
        handle.Free();
        return buffer;
    }

    public void Save(string filename)
    {
        const byte BYTES_PER_PIXEL = 3;
        int row_increment_ = width * BYTES_PER_PIXEL;

        using (FileStream stream = File.Create(filename))
        {
            BitmapInformationHeader bih = new BitmapInformationHeader();
            bih.width = (uint)width;
            bih.height = (uint)height;
            bih.bit_count = BYTES_PER_PIXEL << 3;
            bih.clr_important = 0;
            bih.clr_used = 0;
            bih.compression = 0;
            bih.planes = 1;
            bih.size = (uint)Marshal.SizeOf(bih);
            bih.x_pels_per_meter = 0;
            bih.y_pels_per_meter = 0;
            bih.size_image = (((bih.width * BYTES_PER_PIXEL) + 3) & 0x0000FFFC) * bih.height;

            BitmapFileHeader bfh = new BitmapFileHeader();
            bfh.type = 19778;
            bfh.size = (uint)(Marshal.SizeOf(bfh) + Marshal.SizeOf(bih) + bih.size_image);
            bfh.reserved1 = 0;
            bfh.reserved2 = 0;
            bfh.off_bits = (uint)(Marshal.SizeOf(bih) + Marshal.SizeOf(bfh));

            Debug.LogFormat("Marshal.SizeOf(bfh)={0}", Marshal.SizeOf(bfh));
            Debug.LogFormat("Marshal.SizeOf(bih)={0}", Marshal.SizeOf(bih));

            stream.Write(GetBytes(bfh), 0, Marshal.SizeOf(bfh));
            stream.Write(GetBytes(bih), 0, Marshal.SizeOf(bih));

            int padding = (4 - ((3 * width) % 4)) % 4;
            byte[] padding_data = { 0x00, 0x00, 0x00, 0x00 };

            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    Color32 c = buffer[y * width + x];
                    byte[] bgr = { c.b, c.g, c.r };
                    stream.Write(bgr, 0, 3);
                }
                stream.Write(padding_data, 0, padding);
            }
        }
    }
}
