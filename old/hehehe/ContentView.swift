//
//  ContentView.swift
//  hehehe
//
//  Created by 吳驊祐 on 2021/3/15.
//

import SwiftUI

struct ContentView: View {
    @Binding var document: heheheDocument

    var body: some View {
        TextEditor(text: $document.text)
    }
}

struct ContentView_Previews: PreviewProvider {
    static var previews: some View {
        ContentView(document: .constant(heheheDocument()))
    }
}
