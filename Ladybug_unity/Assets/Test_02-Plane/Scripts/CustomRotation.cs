using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CustomRotation : MonoBehaviour
{
    public Transform lookAtTarget;
    public Light light;
    public MeshFilter meshFilter;

    void PrintVector3(Vector3 v)
    {
        Debug.LogFormat("({0}, {1}, {2})", v.x, v.y, v.z);
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

        Mesh mesh = meshFilter.mesh;
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
            int i0 = i * 3;
            int i1 = i * 3 + 1;
            int i2 = i * 3 + 2;
            Debug.LogFormat("tri {0} {1} {2}", mesh.triangles[i0], mesh.triangles[i1], mesh.triangles[i2]);
        }
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

    }
}
