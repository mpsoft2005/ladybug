using System.Collections;
using System.Collections.Generic;
using System.IO;
using UnityEngine;

public class DebugMesh : MonoBehaviour
{

    public MeshFilter meshFilter;
    private StreamWriter streamWriter;

    private void Awake()
    {
        FileStream stream = File.OpenWrite("Build/unity_debug.log");
        streamWriter = new StreamWriter(stream);
    }

    private void OnDestroy()
    {
        streamWriter.Close();
    }

    void LogFormat(string format, params object[] args)
    {
        string log = string.Format(format, args);
        streamWriter.WriteLine(log);
    }

    public void PrintMesh(Mesh mesh)
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

    // Use this for initialization
    void Start()
    {
        PrintMesh(meshFilter.mesh);
    }

    // Update is called once per frame
    void Update()
    {

    }
}
