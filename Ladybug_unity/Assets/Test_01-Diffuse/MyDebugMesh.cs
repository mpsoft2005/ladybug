using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MyDebugMesh : MonoBehaviour
{

    // Use this for initialization
    void Start()
    {
        MeshFilter meshFilter = GetComponent<MeshFilter>();
        if (meshFilter != null)
        {
            Mesh mesh = meshFilter.sharedMesh;

            Vector3[] verts = mesh.vertices;
            foreach(Vector3 v in verts)
            {
                Debug.LogFormat("{0:g} {1:g} {2:g}", v.x, v.y, v.z);
            }

            int[] tris = mesh.triangles;
            int trisNum = tris.Length / 3;
            for (int i = 0; i < trisNum; i++)
            {
                Debug.LogFormat("{0} {1} {2}", tris[i * 3], tris[i * 3 + 1], tris[i * 3 + 2]);
            }
        }
    }

    // Update is called once per frame
    void Update()
    {

    }
}
