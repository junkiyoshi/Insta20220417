#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofSetCircleResolution(60);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	this->base_seed = glm::vec2(ofRandom(1000), ofRandom(1000));
}

//--------------------------------------------------------------
void ofApp::update() {

	this->mesh.clear();

	int span = 30;
	int radius = 60;
	ofColor color;

	for (int i = 0; i < 3; i++) {

		for (int k = 0; k < 5; k++) {

			auto base_location = glm::vec2(ofMap(ofNoise(this->base_seed.x, ofGetFrameNum() * 0.005), 0, 1, -260, 260), ofMap(ofNoise(this->base_seed.y, ofGetFrameNum() * 0.005), 0, 1, -260, 260));
			auto location = base_location + glm::vec2(radius * cos((ofGetFrameNum() * 5 + i * 120) * DEG_TO_RAD), radius * sin((ofGetFrameNum() * 5 + i * 120) * DEG_TO_RAD));
			auto next = base_location + glm::vec2(radius * cos((((ofGetFrameNum() + 1) * 5) + i * 120) * DEG_TO_RAD), radius * sin((((ofGetFrameNum() + 1) * 5) + i * 120) * DEG_TO_RAD));

			auto distance = location - next;
			distance *= 2;

			auto future = location + distance * 30;
			auto random_deg = ofRandom(360);
			future += glm::vec2(60 * cos(random_deg * DEG_TO_RAD), 60 * sin(random_deg * DEG_TO_RAD));
			auto future_distance = future - location;

			this->location_list.push_back(location);
			this->velocity_list.push_back(glm::normalize(future_distance) * glm::length(distance) * ofRandom(1, 1.5));
			color.setHsb(ofRandom(255), 255, 255);
			this->color_list.push_back(ofColor(color, 0));
		}
	}

	for (int i = this->location_list.size() - 1; i > -1; i--) {

		this->location_list[i] += this->velocity_list[i];
		this->velocity_list[i] *= 1.01;

		if (glm::distance(glm::vec2(), this->location_list[i]) > 1280) {

			this->location_list.erase(this->location_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
		}

		this->mesh.addVertex(glm::vec3(this->location_list[i], 0));
		this->mesh.addColor(this->color_list[i]);
	}

	for (int i = 0; i < this->mesh.getNumVertices(); i++) {

		for (int k = i + 1; k < this->mesh.getNumVertices(); k++) {

			auto distance = glm::distance(this->mesh.getVertex(i), this->mesh.getVertex(k));
			if (distance < span) {

				auto alpha = distance < span * 0.25 ? 255 : ofMap(distance, span * 0.25, span, 255, 0);

				if (this->mesh.getColor(i).a < alpha) {

					this->mesh.setColor(i, ofColor(this->mesh.getColor(i), alpha));
				}

				if (this->mesh.getColor(k).a < alpha) {

					this->mesh.setColor(k, ofColor(this->mesh.getColor(k), alpha));
				}

				this->mesh.addIndex(i);
				this->mesh.addIndex(k);
			}
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
	
	this->mesh.drawWireframe();

	ofSetColor(255);
	ofNoFill();
	int radius = 60;
	vector<glm::vec2> vertices;
	for (int i = 0; i < 3; i++) {

		auto base_location = glm::vec2(ofMap(ofNoise(this->base_seed.x, ofGetFrameNum() * 0.005), 0, 1, -260, 260), ofMap(ofNoise(this->base_seed.y, ofGetFrameNum() * 0.005), 0, 1, -260, 260));
		auto location = base_location + glm::vec2(radius * cos((ofGetFrameNum() * 5 + i * 120 - 4 ) * DEG_TO_RAD), radius * sin((ofGetFrameNum() * 5 + i * 120 - 4) * DEG_TO_RAD));
		vertices.push_back(location);
	}

	ofBeginShape();
	ofVertices(vertices);
	ofEndShape(true);

	for (auto& vertex : vertices) {

		ofSetColor(255);
		ofDrawCircle(vertex, 8);
	}

	ofFill();
	for (int i = 0; i < this->mesh.getNumVertices(); i++) {

		ofSetColor(this->mesh.getColor(i));
		ofDrawCircle(this->mesh.getVertex(i), 2);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}