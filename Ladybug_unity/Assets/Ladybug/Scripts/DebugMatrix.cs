using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DebugMatrix : MonoBehaviour
{
    private const string matrixFormat =
        "{0,13} {1,13} {2,13} {3,13}\n" +
        "{4,13} {5,13} {6,13} {7,13}\n" +
        "{8,13} {9,13} {10,13} {11,13}\n" +
        "{12,13} {13,13} {14,13} {15,13}";

    // Use this for initialization
    void Start()
    {
        Matrix4x4 m;
        Camera c = Camera.main;

        m = c.cameraToWorldMatrix;
        Debug.LogFormat("cameraToWorldMatrix:");
        Debug.LogFormat(matrixFormat,
            m.m00, m.m01, m.m02, m.m03,
            m.m10, m.m11, m.m12, m.m13,
            m.m20, m.m21, m.m22, m.m23,
            m.m30, m.m31, m.m32, m.m33
        );

        m = c.worldToCameraMatrix;
        Debug.LogFormat("View Matrix (worldToCameraMatrix):");
        Debug.LogFormat(matrixFormat,
            m.m00, m.m01, m.m02, m.m03,
            m.m10, m.m11, m.m12, m.m13,
            m.m20, m.m21, m.m22, m.m23,
            m.m30, m.m31, m.m32, m.m33
        );

        m = c.projectionMatrix;
        Debug.LogFormat("Projection Matrix:");
        Debug.LogFormat(matrixFormat,
            m.m00, m.m01, m.m02, m.m03,
            m.m10, m.m11, m.m12, m.m13,
            m.m20, m.m21, m.m22, m.m23,
            m.m30, m.m31, m.m32, m.m33
        );
    }

    // Update is called once per frame
    void Update()
    {

    }
}
