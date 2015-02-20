
using System;
using UnityEngine;

public class DynamicObject : MonoBehaviour
{
	private float m_life_time = 1 * 5;
	private float m_radius = 1;
	public Vector3 m_velocity = new Vector3();

	void Start()
	{
		Destroy(this.gameObject, m_life_time);
	}

	void Update()
	{
		this.transform.position += m_velocity * Time.deltaTime;
	}
}
