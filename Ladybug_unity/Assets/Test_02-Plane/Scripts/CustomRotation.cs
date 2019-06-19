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
            Debug.LogFormat("v {0} {1} {2}", v.x, v.y, v.z);
        }

        for (int i = 0; i < mesh.uv.Length; i++)
        {
            Vector2 vt = mesh.uv[i];
            Debug.LogFormat("vt {0} {1}", vt.x, vt.y);
        }

        for (int i = 0; i < mesh.normals.Length; i++)
        {
            Vector3 vn = mesh.normals[i];
            Debug.LogFormat("vn {0} {1} {2}", vn.x, vn.y, vn.z);
        }

        for (int i = 0; i < mesh.triangles.Length / 3; i++)
        {
            int i0 = mesh.triangles[i * 3];
            int i1 = mesh.triangles[i * 3 + 1];
            int i2 = mesh.triangles[i * 3 + 2];

            Debug.LogFormat("tri {0} {1} {2}", i0, i1, i2);

            Debug.LogFormat("tri ({0} {1} {2})/({3} {4})/({5} {6} {7})  ({8} {9} {10})/({11} {12})/({13} {14} {15})  ({16} {17} {18})/({19} {20})/({21} {22} {23})",
                mesh.vertices[i0].x, mesh.vertices[i0].y, mesh.vertices[i0].z, mesh.uv[i0].x, mesh.uv[i0].y, mesh.normals[i0].x, mesh.normals[i0].y, mesh.normals[i0].z,
                mesh.vertices[i1].x, mesh.vertices[i1].y, mesh.vertices[i1].z, mesh.uv[i1].x, mesh.uv[i1].y, mesh.normals[i1].x, mesh.normals[i1].y, mesh.normals[i1].z,
                mesh.vertices[i2].x, mesh.vertices[i2].y, mesh.vertices[i2].z, mesh.uv[i2].x, mesh.uv[i2].y, mesh.normals[i2].x, mesh.normals[i2].y, mesh.normals[i2].z);
        }
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

            byte[] raw = texture.EncodeToJPG();
            File.WriteAllBytes("mytest.jpg", raw);
        }
    }
}
