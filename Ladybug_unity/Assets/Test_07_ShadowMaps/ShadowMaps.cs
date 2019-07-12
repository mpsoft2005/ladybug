using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShadowMaps : MonoBehaviour
{
    public Transform light;

    // Use this for initialization
    void Start()
    {
        Vector3 vWorld = new Vector3(3, 4, 5);
        Vector3 vRaster = Camera.main.WorldToScreenPoint(vWorld);
        Debug.Log("vWorld");
        Debug.Log(vRaster);

        QualitySettings.shadows = ShadowQuality.All;

        Vector3 lightDir = -light.forward;
        Debug.Log("lightDir");
        Debug.LogFormat("{0}, {1}, {2}", lightDir.x, lightDir.y, lightDir.z);
    }

    // Update is called once per frame
    void Update()
    {

    }
}
