Shader "Custom/DoubleSidedTransparent"
{
    Properties
	{
        _FrontColor ("Front Color", Color) = (1, 1, 1, 1)
        _BackColor ("Back Color", Color) = (1, 1, 1, 1)
    }
	
    SubShader
	{
        Tags { "Queue" = "Transparent" }
        LOD 200

		Pass
        {
            Blend SrcAlpha OneMinusSrcAlpha
            ZWrite Off
            Cull Back
			
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #include "UnityCG.cginc"

            uniform float4 _FrontColor;

            float4 vert(float4 vertPos : POSITION) : SV_POSITION
            {
                return UnityObjectToClipPos(vertPos);
            }

            float4 frag(float4 vertPos : SV_POSITION) : COLOR
            {
                return _FrontColor;
            }
            ENDCG
        }
		
        Pass
        {
            Blend SrcAlpha OneMinusSrcAlpha
            ZWrite Off
            Cull Front
			
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #include "UnityCG.cginc"

            uniform float4 _BackColor;

            float4 vert(float4 vertPos : POSITION) : SV_POSITION
            {
                return UnityObjectToClipPos(vertPos);
            }

            float4 frag(float4 vertPos : SV_POSITION) : COLOR
            {
                return _BackColor;
            }
            ENDCG
        }
    }
    FallBack "Diffuse"
}
