using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TestMatrixInverse : MonoBehaviour
{

    // Use this for initialization
    void Start()
    {
        Matrix4x4 translationMatrix = new Matrix4x4(
            new Vector4(1, 0, 0, 0),
            new Vector4(0, 1, 0, 0),
            new Vector4(0, 0, 1, 0),
            new Vector4(1, 2, 3, 1)
        );
        Debug.Log("translationMatrix is:");
        Debug.Log(translationMatrix);

        Matrix4x4 rotationMatrix = new Matrix4x4(
            new Vector4(0.8434932f, 0.492404f, -0.21461f, 0),
            new Vector4(-0.4184121f, 0.8528684f, 0.3123246f, 0),
            new Vector4(0.336824f, -0.1736483f, 0.9254165f, 0),
            new Vector4(0, 0, 0, 1)
        );
        Debug.Log("rotationMatrix is:");
        Debug.Log(rotationMatrix);

        Matrix4x4 scalingMatrix = new Matrix4x4(
            new Vector4(0.1f, 0, 0, 0),
            new Vector4(0, 0.2f, 0, 0),
            new Vector4(0, 0, 0.3f, 0),
            new Vector4(0, 0, 0, 1)
        );
        Debug.Log("scalingMatrix is:");
        Debug.Log(scalingMatrix);

        Matrix4x4 transformationMatrix = translationMatrix * rotationMatrix * scalingMatrix;

        Debug.Log("transformationMatrix is:");
        Debug.Log(transformationMatrix);

        Debug.Log("transformationMatrix.inverse() is:");
        Debug.Log(transformationMatrix.inverse);
    }

    // Update is called once per frame
    void Update()
    {

    }
}
