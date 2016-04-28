using UnityEngine;
using UnityEngine.UI;

namespace DerelictComputer
{
    public class QuinoaUIControl : MonoBehaviour
    {
        [SerializeField] private Quinoa _quinoa;

        public void SetSpeed(float speed)
        {
            _quinoa.Speed = speed;
        }

        public void SetPitch(float pitch)
        {
            _quinoa.Pitch = pitch;
        }

        public void SetStart(float start)
        {
            _quinoa.Start = start;
        }

        public void SetLength(float length)
        {
            _quinoa.Length = length;
        }

        public void SetSmoothness(float smoothness)
        {
            _quinoa.Smoothness = smoothness;
        }

        public void SetWindowSize(float windowSize)
        {
            _quinoa.WindowSize = (int)windowSize;
        }
    }
}
