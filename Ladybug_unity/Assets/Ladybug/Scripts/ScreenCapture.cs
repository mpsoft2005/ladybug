using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ScreenCapture : MonoBehaviour
{
    public bool capture;
    public string captureFilename = "capture.bmp";

    // Update is called once per frame
    void Update()
    {
        if (capture)
        {
            capture = false;

            RenderTexture oldTexture;
            RenderTexture renderTexture = new RenderTexture(Screen.width, Screen.height, 24);

            oldTexture = Camera.main.targetTexture;
            Camera.main.targetTexture = renderTexture;
            Camera.main.Render();
            Camera.main.targetTexture = oldTexture;

            oldTexture = RenderTexture.active;
            RenderTexture.active = renderTexture;
            Texture2D texture = new Texture2D(renderTexture.width, renderTexture.height);
            texture.ReadPixels(new Rect(0, 0, renderTexture.width, renderTexture.height), 0, 0);
            texture.Apply();
            RenderTexture.active = oldTexture;

            Bitmap bitmap = new Bitmap(Screen.width, Screen.height);
            bitmap.SetPixels(texture.GetPixels32());
            bitmap.Save(captureFilename);
        }
    }
}
