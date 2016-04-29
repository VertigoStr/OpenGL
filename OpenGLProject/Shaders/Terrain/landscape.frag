#version 120

uniform float HEIGHTMAP_SCALE;
uniform sampler2D region1ColorMap;
uniform sampler2D region2ColorMap;
uniform sampler2D region3ColorMap;
uniform sampler2D region4ColorMap;

varying vec2 texCoord;
varying vec4 normal;

vec4 GenerateTerrainColor()
{
    vec4 terrainColor = vec4(0.0, 0.0, 0.0, 1.0);
    float height = normal.w;
    float regionMin = 0.0;
    float regionMax = 0.0;
    float regionRange = 0.0;
    float regionWeight = 0.0;
    
    // Terrain region 1.
    regionMin = -12.5f * HEIGHTMAP_SCALE;
    regionMax = -6.9f * HEIGHTMAP_SCALE;
    regionRange = regionMax - regionMin;
    regionWeight = (regionRange - abs(height - regionMax)) / regionRange;
    regionWeight = max(0.0, regionWeight);
    terrainColor += regionWeight * texture2D(region1ColorMap, texCoord);

    // Terrain region 2.
    regionMin = -6.901f * HEIGHTMAP_SCALE;
    regionMax = 3.0f * HEIGHTMAP_SCALE;
    regionRange = regionMax - regionMin;
    regionWeight = (regionRange - abs(height - regionMax)) / regionRange;
    regionWeight = max(0.0, regionWeight);
    terrainColor += regionWeight * texture2D(region2ColorMap, texCoord);

    // Terrain region 3.
    regionMin = 3.1f * HEIGHTMAP_SCALE;
    regionMax = 18.0f * HEIGHTMAP_SCALE;
    regionRange = regionMax - regionMin;
    regionWeight = (regionRange - abs(height - regionMax)) / regionRange;
    regionWeight = max(0.0, regionWeight);
    terrainColor += regionWeight * texture2D(region3ColorMap, texCoord);

    // Terrain region 4.
    regionMin = 18.1 * HEIGHTMAP_SCALE;
    regionMax = 2500.1f * HEIGHTMAP_SCALE;
    regionRange = regionMax - regionMin;
    regionWeight = (regionRange - abs(height - regionMax)) / regionRange;
    regionWeight = max(0.0, regionWeight);
    terrainColor += regionWeight * texture2D(region4ColorMap, texCoord);

    return terrainColor;
}

void main()
{
	gl_FragColor = GenerateTerrainColor() * vec4(0.2f,0.2f,0.2f,1.0f);
}