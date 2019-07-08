using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShadowMaps : MonoBehaviour
{

    // Use this for initialization
    void Start()
    {
        Vector3 vWorld = new Vector3(3, 4, 5);
        Vector3 vRaster = Camera.main.WorldToScreenPoint(vWorld);
        Debug.Log("vWorld");
        Debug.Log(vRaster);
    }

    // Update is called once per frame
    void Update()
    {

    }
}
