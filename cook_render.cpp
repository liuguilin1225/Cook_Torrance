

	material_coeff[8]; //cook torrance material coefficients

	dr = material_coeff[0];
	dg = material_coeff[1];
	db = material_coeff[2];

	sr = material_coeff[3];
	sg = material_coeff[4];
	sb = material_coeff[5];

	F0 = material_coeff[6];
	m = material_coeff[7];

	///////////////////////////////////////////////////////////

	vec3 lightDirection;
	vec3 varNormal;
	vec3 varEyeDir;
	vec3 lightColor;

    // interpolating normals will change the length of the normal, so renormalize the normal.
    vec3 normal = normalize(varNormal);

    // do the lighting calculation for each fragment.
    float NdotL = max(dot(normal, lightDirection), 0.0);

    float specular = 0.0;

    if(NdotL > 0.0)
    {
        vec3 eyeDir = normalize(varEyeDir);

        // calculate intermediary values
        vec3 halfVector = normalize(lightDirection + eyeDir);
        float NdotH = max(dot(normal, halfVector), 0.0); 
        float NdotV = max(dot(normal, eyeDir), 0.0); // note: this could also be NdotL, which is the same value
        float VdotH = max(dot(eyeDir, halfVector), 0.0);
        float mSquared = roughnessValue * roughnessValue;

        // geometric attenuation
        float NH2 = 2.0 * NdotH;
        float g1 = (NH2 * NdotV) / VdotH;
        float g2 = (NH2 * NdotL) / VdotH;
        float geoAtt = min(1.0, min(g1, g2));

        // roughness (or: microfacet distribution function)
        // beckmann distribution function

        float r1 = 1.0 / ( mSquared * pow(NdotH, 4.0));
        float r2 = (NdotH * NdotH - 1.0) / (mSquared * NdotH * NdotH);
        float roughness = r1 * exp(r2);

        // fresnel
        // Schlick approximation
        float fresnel = pow(1.0 - VdotH, 5.0);
        fresnel *= (1.0 - F0);
        fresnel += F0;

        specular = (fresnel * geoAtt * roughness) / (NdotV * NdotL * 3.14);

        // are the following correct ???
        final_color[0] = lightColor[0] * NdotL * (dr + sr * specular);
        final_color[1] = lightColor[1] * NdotL * (dr + sr * specular);
        final_color[2] = lightColor[2] * NdotL * (dr + sr * specular);
    }
