﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Walk : MonoBehaviour
{
    Motion motion;
    
    float[] maxAngle;
    // Use this for initialization
    void Start()
    {
        motion = gameObject.GetComponent<Motion>();
        //motion.Init();
        State[] state = motion.state;
        Debug.Log(name + " states: " + state.Length);
        for(int i=0;i<4;i++)
        {
            if(i ==0||i==2)
            {
                state[i].isRightStance = i == 0 ? true : false;
                state[i].torso = 0;
                state[i].swingHip = 0.4f;
                state[i].swingKnee = -1.1f;
                state[i].swingAnkle = 0.2f;
                state[i].stanceKnee = -0.05f;
                state[i].stanceAnkle = 0.2f; //0.2f
                state[i].duration = 0.3f; // 0.3f;
                state[i].cd = 0;
                state[i].cv = 0.1f; //0.2f
            }
            else
            {
                state[i].isRightStance = i == 1 ? true : false;
                state[i].torso = 0;
                state[i].swingHip = -0.7f;
                state[i].swingKnee = -0.05f;
                state[i].swingAnkle = 0.2f;
                state[i].stanceKnee = -0.10f;
                state[i].stanceAnkle = 0.2f; //0.2f
                state[i].duration = 0f;
                state[i].cd = 2.20f;
                state[i].cv = 0;

            }
            state[i].Init();
        }

        motion.targetVelocity = 1f;

        motion.minAngle[0] = -Mathf.PI * 0.5f; //torso
        motion.maxAngle[0] = Mathf.PI * 0.5f;
        motion.minAngle[1] = -Mathf.PI * 0.5f; //right hip
        motion.maxAngle[1] = Mathf.PI * 0.5f;
        motion.minAngle[2] = -Mathf.PI * 0.5f; //left hip
        motion.maxAngle[2] = Mathf.PI * 0.5f;
        motion.minAngle[3] = -Mathf.PI + 0.05f; //right knee
        motion.maxAngle[3] = -0.00f;
        motion.minAngle[4] = -Mathf.PI + 0.05f; //left knee
        motion.maxAngle[4] = -0.00f;
        motion.minAngle[5] = -Mathf.PI * 0.5f + 0.05f; //right ankle
        motion.maxAngle[5] = Mathf.PI * 0.5f;
        motion.minAngle[6] = -Mathf.PI * 0.5f + 0.05f; //left ankle
        motion.maxAngle[6] = Mathf.PI * 0.5f;

    }

}
