using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CustomRotation : MonoBehaviour
{
    public Transform lookAtTarget;

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
