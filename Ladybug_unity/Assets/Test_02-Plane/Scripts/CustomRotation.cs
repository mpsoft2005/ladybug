using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class CustomRotation : MonoBehaviour
{
    public Transform lookAtTarget;
    public Light light;
    public MeshFilter meshFilter;
    public bool capture;

    private StreamWriter streamWriter;

    void PrintVector3(Vector3 v)
    {
        Debug.LogFormat("({0}, {1}, {2})", v.x, v.y, v.z);
    }

    void DebugMesh(Mesh mesh)
    {
        Debug.Log("mesh.vertices.Length=" + mesh.vertices.Length);
        Debug.Log("mesh.normals.Length=" + mesh.normals.Length);
        Debug.Log("mesh.triangles.Length=" + mesh.triangles.Length);
        Debug.Log("mesh.uv.Length=" + mesh.uv.Length);

        for (int i = 0; i < mesh.vertices.Length; i++)
        {
            Vector3 v = mesh.vertices[i];
            LogFormat("v {0:N6} {1:N6} {2:N6}", v.x, v.y, v.z);
        }

        for (int i = 0; i < mesh.uv.Length; i++)
        {
            Vector2 vt = mesh.uv[i];
            LogFormat("vt {0:N6} {1:N6}", vt.x, vt.y);
        }

        for (int i = 0; i < mesh.normals.Length; i++)
        {
            Vector3 vn = mesh.normals[i];
            LogFormat("vn {0:N6} {1:N6} {2:N6}", vn.x, vn.y, vn.z);
        }

        for (int i = 0; i < mesh.triangles.Length / 3; i++)
        {
            int i0 = mesh.triangles[i * 3];
            int i1 = mesh.triangles[i * 3 + 1];
            int i2 = mesh.triangles[i * 3 + 2];
            LogFormat("f {0} {1} {2}", i0, i1, i2);
        }
    }

    void LogFormat(string format, params object[] args)
    {
        string log = string.Format(format, args);
        streamWriter.WriteLine(log);
    }

    private void Awake()
    {
        FileStream stream = File.OpenWrite("ladybug.log");
        streamWriter = new StreamWriter(stream);
    }

    private void OnDestroy()
    {
        streamWriter.Close();
    }

    // Use this for initialization
    void Start()
    {
        Debug.Log("View Matrix:");
        Debug.Log(Camera.main.worldToCameraMatrix);
        Debug.Log("Projection Matrix:");
        Debug.Log(Camera.main.projectionMatrix);

        Matrix4x4 mvpMatrix = Camera.main.projectionMatrix * Camera.main.worldToCameraMatrix * Matrix4x4.identity;
        Debug.Log("MVP Matrix:");
        Debug.Log(mvpMatrix);

        Vector3 worldPoint = new Vector3(1.000000f, -1.000000f, 5.000000f);
        Vector3 viewportPoint = Camera.main.WorldToViewportPoint(worldPoint);
        Debug.Log("Viewport Point:");
        Debug.Log(viewportPoint);

        Vector3 screenPoint = Camera.main.WorldToScreenPoint(worldPoint);
        Debug.Log("Screen Point:");
        Debug.Log(screenPoint);

        TestRay(0, 0);
        TestRay(100, 100);
        TestRay(640, 480);

        Debug.Log("Light dir(world space):");
        PrintVector3(light.transform.forward);

        DebugMesh(meshFilter.mesh);
    }

    void TestRay(int x, int y)
    {
        Ray ray = Camera.main.ScreenPointToRay(new Vector3(x, y, 0));
        ray.origin = Camera.main.worldToCameraMatrix.MultiplyPoint(ray.origin);
        ray.direction = Camera.main.worldToCameraMatrix.MultiplyVector(ray.direction);

        Debug.Log("Ray origin(camera space):");
        PrintVector3(ray.origin);

        Debug.Log("Ray dir(camera space):");
        PrintVector3(ray.direction);
    }

    // Update is called once per frame
    void Update()
    {
        if (capture)
        {
            capture = false;

            RenderTexture oldTexture;
            RenderTexture renderTexture = new RenderTexture(640, 480, 24);

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

            Bitmap bitmap = new Bitmap(640, 480);
            bitmap.SetPixels(texture.GetPixels32());
            bitmap.Save("mytest.bmp");
        }
    }
}
