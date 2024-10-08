import sys
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QPushButton, QTabWidget, QLabel, QComboBox, QFileDialog, QScrollArea, QMessageBox
from PyQt5.QtGui import QPixmap, QColor

class PowerPointGenerator(QWidget):
    def __init__(self):
        super().__init__()
        self.init_ui()
        self.slides = []
        self.slide_count = 0
        self.finding_proposals = {
    'No IPC Class Level Stated': ['Proposal: Customer to provide IPC Class Level of Product'],
    'PCBA is a product component': ['Customer to provide AVL', 'Internal:Require EPA Assembly'],
    'Cable is a product component':['Customer to provide AVl','Intenal:IQC Check to Cater to IPC LEvel'],
    'Product require functional test':['Customer to Provide Test Fixture Setup','UWC to Produce Test Fixture for Customer'],
    'Product require Hi-Pot test':['Customer to define Hi-Pot Test Specification and Setup'],
    'Product require Continuity test':['Customer to provide complete schematic and continuity test point',"UWC to decide on the Continuity Test Point and to be verified by Customer Side"],
    'Product involve Spftware/Firmware/Hardware for Testing/Shipping':['Customer to provide required Software/Firmware/Hardware as listed in BOM'],
    'Product uses Custom part from supplier':['Customer to provide AVL','Internal:TO compare technical data sheet provided by AVL with incumbent'],
    'Product involves Electrical part to be attached on Mechanical Part':['Internal:NPI to be more Aware of the tolerance/dimension at Mechanical Attach Point of Electrical Component'],
    'Problem with Voltage Distribution Analysis':['Customer to Advise the Unmatched Voltage Level and/or Type'],
    'Problem with Power Load Analysis':['Customer to Advise Power Supply and Load Non-Balance Issue'],
    'Connector/Crimp used for Cable does not match mating part':['Customer to advice on the Actual Connector/Crimp required','UWC To Propose Suitable Crimp/Connector based on the Connection Point'],
    # Map more findings to their proposal options as needed
}

    def init_ui(self):
        self.setWindowTitle('DFM Report Generator')

        layout = QVBoxLayout()
        self.setStyleSheet("background-color: cyan; color: black;")  # Setting background and text color

        tab_widget = QTabWidget()

        slide_tab = QWidget()
        self.slide_scroll = QScrollArea()
        self.slide_scroll.setWidgetResizable(True)
        self.slide_scroll_widget = QWidget()
        self.slide_layout = QVBoxLayout(self.slide_scroll_widget)
        self.slide_scroll.setWidget(self.slide_scroll_widget)

        self.add_slide_button = QPushButton('Add Part')
        self.add_slide_button.clicked.connect(self.add_slide)
        self.add_slide_button.setStyleSheet("background-color: yellow; color: black;")  # Setting button color
        self.slide_layout.addWidget(self.add_slide_button)

        self.preview_button = QPushButton('Preview Proposal')
        self.preview_button.clicked.connect(self.preview_slide)
        self.preview_button.setStyleSheet("background-color: yellow; color: black;")  # Setting button color
        self.slide_layout.addWidget(self.preview_button)

        self.publish_button = QPushButton('Publish Slide')
        self.publish_button.clicked.connect(self.publish_slide)
        self.publish_button.setStyleSheet("background-color: yellow; color: black;")  # Setting button color
        self.slide_layout.addWidget(self.publish_button)

        tab_widget.addTab(self.slide_scroll, 'Add Part')

        layout.addWidget(tab_widget)
        self.setLayout(layout)
        self.show()

    def add_slide(self):
        self.slide_count += 1

        slide_label = QLabel(f"Slide {self.slide_count}")
        self.slide_layout.addWidget(slide_label)

        finding_combo = QComboBox()
        finding_combo.addItem('Select a finding')
        finding_combo.addItem('No IPC Class Level Stated')
        finding_combo.addItem('PCBA is a product component')
        finding_combo.addItem('Cable is a product component')
        finding_combo.addItem('Product require functional test')
        finding_combo.addItem('Product require Hi-Pot test')
        finding_combo.addItem('Product require Continuity test')
        finding_combo.addItem('Product involve Spftware/Firmware/Hardware for Testing/Shipping')
        finding_combo.addItem('Product uses Custom part from supplier')
        finding_combo.addItem('Product involves Electrical part to be attached on Mechanical Part')
        finding_combo.addItem('Problem with Voltage Distribution Analysis')
        finding_combo.addItem('Problem with Power Load Analysis')
        finding_combo.addItem('Connector/Crimp used for Cable does not match mating part')
        finding_combo.currentIndexChanged.connect(lambda: self.show_proposals(finding_combo.currentText()))
        self.slide_layout.addWidget(finding_combo)

        proposal_combo = QComboBox()
        proposal_combo.addItem('Select a proposal')
        proposal_combo.setObjectName(f"proposal_combo_{self.slide_count}")
        self.slide_layout.addWidget(proposal_combo)

        attach_picture_button = QPushButton(f'Attach Picture to Slide {self.slide_count}')
        attach_picture_button.clicked.connect(lambda: self.attach_picture(self.slide_count))  # Connect button to attach_picture method
        attach_picture_button.setStyleSheet("background-color: yellow; color: black;")  # Setting button color
        self.slide_layout.addWidget(attach_picture_button)

        picture_preview = QLabel()  # To display picture preview
        picture_preview.setFixedSize(200, 200)
        self.slide_layout.addWidget(picture_preview)

    def show_proposals(self, finding):
        proposal_combo = self.sender().parent().findChild(QComboBox, f"proposal_combo_{self.slide_count}")
        proposal_combo.clear()
        proposal_combo.addItem('Select a proposal')
        proposals = self.finding_proposals.get(finding, [])
        proposal_combo.addItems(proposals)

    def preview_slide(self):
        current_slide = self.slide_layout.itemAt(self.slide_layout.count() - 5).widget()
        #finding = current_slide.findChild(QComboBox).currentText()
        for i in range(self.slide_layout.count()):
            widget = self.slide_layout.itemAt(i).widget()
            if isinstance(widget, QComboBox) and widget.objectName().startswith("proposal_combo_"):
                finding_combo = self.slide_layout.itemAt(i - 1).widget()
                finding = finding_combo.currentText()
                proposal = widget.currentText()
        # Rest of your logic using finding and proposal...

        proposal = current_slide.findChild(QComboBox, f"proposal_combo_{self.slide_count}").currentText()

        if finding != 'Select a finding' and proposal != 'Select a proposal':
            QMessageBox.information(self, "Slide Preview", f"Finding: {finding}\nProposal: {proposal}")
        else:
            QMessageBox.warning(self, "Incomplete Slide", "Please select a finding and a proposal.")

    def publish_slide(self):
        current_slide = self.slide_layout.itemAt(self.slide_layout.count() - 5).widget()
        finding = current_slide.findChild(QComboBox).currentText()
        proposal = current_slide.findChild(QComboBox, f"proposal_combo_{self.slide_count}").currentText()

        if finding != 'Select a finding' and proposal != 'Select a proposal':
            QMessageBox.information(self, "Slide Published", f"Slide with Finding: {finding}\nProposal: {proposal} has been published.")
        else:
            QMessageBox.warning(self, "Incomplete Slide", "Please select a finding and a proposal.")

    def attach_picture(self, slide_number):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        file_name, _ = QFileDialog.getOpenFileName(self, "Select Image", "", "Image Files (*.png *.jpg *.jpeg *.gif)", options=options)

        if file_name:
            current_slide = self.slide_layout.itemAt(self.slide_layout.count() - 5).widget()

            picture_preview = QLabel()  # Create a QLabel for the picture preview
            picture_preview.setFixedSize(200, 200)
            pixmap = QPixmap(file_name)
            picture_preview.setPixmap(pixmap.scaled(200, 200, aspectRatioMode=True))

            self.slide_layout.insertWidget(self.slide_layout.count() - 1, picture_preview)  # Insert picture preview before the attachment button
        else:
            QMessageBox.warning(self, "Error", "No image selected.")
    def generate_pptx(self):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        file_dialog = QFileDialog()
        file_dialog.setOptions(options)
        file_dialog.setFileMode(QFileDialog.AnyFile)
        file_dialog.setNameFilter("PowerPoint Files (*.pptx)")
        file_dialog.setDefaultSuffix("pptx")
        file_dialog.setAcceptMode(QFileDialog.AcceptSave)

        if file_dialog.exec_():
            file_paths = file_dialog.selectedFiles()
        if file_paths:
            file_path = file_paths[0]
            prs = Presentation()

            for slide_number in self.slides_widgets:
                # Retrieve slide information here as before
                # For each slide, create a slide in the presentation
                slide = prs.slides.add_slide(prs.slide_layouts[0])  # Change the slide_layout index as needed

                # Add content to the slide (finding, proposal, picture, etc.)
                # Use slide placeholders or shapes to add your content

            # Save the presentation
            prs.save(file_path)
            QMessageBox.information(self, "Presentation Created", f"PowerPoint presentation created successfully and saved at: {file_path}")


def run_app():
    app = QApplication(sys.argv)
    ex = PowerPointGenerator()
    sys.exit(app.exec_())

if __name__ == '__main__':
    run_app()
